#include "LogicSystem.h"
#include "json.hpp"

using json = nlohmann::json;

LogicSystem::~LogicSystem()
{
    stop_ = true;
    cond_.notify_one();
    workerThread_.join();
}

void LogicSystem::postMsgToQue(std::shared_ptr<LogicNode> msg)
{
    std::unique_lock<std::mutex> lock(mutex_);
    que_.push(msg);

    if (que_.size() == 1)
    {
        cond_.notify_one();
    }
}

LogicSystem::LogicSystem() : stop_(false)
{
    registerCallBack();
    workerThread_ = std::thread(&LogicSystem::dealMsg, this);
}

void LogicSystem::registerCallBack()
{
    funCallBacK_.emplace(std::make_pair(MSG_HELLO_WORD, std::bind(&LogicSystem::helloWorldCallback, this,
                                                                  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
}

void LogicSystem::helloWorldCallback(std::shared_ptr<CSession> session, const short &msgId, const std::string &msgData)
{
    json root = json::parse(msgData);
    std::cout << "receive msg id is " << root["id"].get<int>() << " msg data is " << root["data"].get<std::string>() << std::endl;
    root["data"] = "server has receive msg. msg data is " + root["data"].get<std::string>();
    std::string rStr = root.dump();
    session->Send(rStr, root["id"].get<int>());
}

void LogicSystem::dealMsg()
{
    while (1)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [&]()
                   { return !que_.empty() || stop_; });

        // 停服
        if (stop_)
        {
            while (!que_.empty())
            {
                auto msgNode = que_.front();
                std::cout << "recv msg id is " << msgNode->recvnode_->getMsgId() << std::endl;
                auto it = funCallBacK_.find(msgNode->recvnode_->getMsgId());
                if (it == funCallBacK_.end())
                {
                    que_.pop();
                    continue;
                }
                it->second(msgNode->session_, msgNode->recvnode_->getMsgId(),
                           std::string(msgNode->recvnode_->_data, msgNode->recvnode_->_cur_len));
                que_.pop();
                break;
            }
        }
        // 未停服
        auto msgNode = que_.front();
        std::cout << "recv msg id is " << msgNode->recvnode_->getMsgId() << std::endl;
        auto it = funCallBacK_.find(msgNode->recvnode_->getMsgId());
        if (it == funCallBacK_.end())
        {
            que_.pop();
            continue;
        }
        it->second(msgNode->session_, msgNode->recvnode_->getMsgId(),
                   std::string(msgNode->recvnode_->_data, msgNode->recvnode_->_cur_len));
        que_.pop();
    }
}
