#include "session.h"

#include <functional>

Session::Session(std::shared_ptr<boost::asio::ip::tcp::socket> socket) : sock_(socket), sendPending_(false), _recv_pending(false) {}

void Session::connect(const boost::asio::ip::tcp::endpoint &ep)
{
    sock_->connect(ep);
}

void Session::writeCallBackErr(const boost::system::error_code &ec, std::size_t byteTransferred, std::shared_ptr<MsgNode> msgNode)
{
    if (byteTransferred + msgNode->curLen_ < msgNode->totalLen_)
    {
        sendNode_->curLen_ += byteTransferred;
        sock_->async_write_some(boost::asio::buffer(sendNode_->msg_ + sendNode_->curLen_, sendNode_->totalLen_ - sendNode_->curLen_),
                                std::bind(&Session::writeCallBackErr, this,
                                          std::placeholders::_1, std::placeholders::_2, sendNode_));
    }
}

void Session::wirteToSocketErr(const std::string buf)
{
    sendNode_ = std::make_shared<MsgNode>(buf.c_str(), buf.length());
    sock_->async_write_some(boost::asio::buffer(sendNode_->msg_, sendNode_->totalLen_),
                            std::bind(&Session::writeCallBackErr, this,
                                      std::placeholders::_1, std::placeholders::_2, sendNode_));
}

void Session::writeCallBack(const boost::system::error_code &ec, std::size_t byteTransferred)
{
    if (ec.value() != 0)
    {
        std::cout << "Error,code is " << ec.value() << ". Message is " << ec.message() << std::endl;
        return;
    }
    auto &sendData = sendQueue.front();
    sendData->curLen_ += byteTransferred;
    if (sendData->curLen_ < sendData->totalLen_)
    {
        sock_->async_write_some(boost::asio::buffer(sendNode_->msg_ + sendNode_->curLen_, sendNode_->totalLen_ - sendNode_->curLen_),
                                std::bind(&Session::writeCallBack, this,
                                          std::placeholders::_1, std::placeholders::_2));
        return;
    }

    sendQueue.pop();

    if (sendQueue.empty())
    {
        sendPending_ = false;
    }

    if (!sendQueue.empty())
    {
        auto &sendData = sendQueue.front();
        sock_->async_write_some(boost::asio::buffer(sendData->msg_ + sendData->curLen_, sendData->totalLen_ - sendData->curLen_),
                                std::bind(&Session::writeCallBack, this, std::placeholders::_1, std::placeholders::_2));
    }
}

void Session::wirteToSocket(const std::string buf)
{
    sendQueue.emplace(std::make_shared<MsgNode>(buf.c_str(), buf.length()));
    if (sendPending_)
    {
        return;
    }
    sock_->async_write_some(boost::asio::buffer(buf),
                            std::bind(&Session::writeCallBack, this,
                                      std::placeholders::_1, std::placeholders::_2));
    sendPending_ = true;
}

void Session::writeAllCallBack(const boost::system::error_code &ec, std::size_t byteTransferred)
{
    if (ec.value() != 0)
    {
        std::cout << "Error,code is " << ec.value() << ". Message is " << ec.message() << std::endl;
        return;
    }

    sendQueue.pop();
    if (sendQueue.empty())
    {
        sendPending_ = false;
    }

    if (!sendQueue.empty())
    {
        auto &sendData = sendQueue.front();
        sock_->async_write_some(boost::asio::buffer(sendData->msg_ + sendData->curLen_, sendData->totalLen_ - sendData->curLen_),
                                std::bind(&Session::writeCallBack, this, std::placeholders::_1, std::placeholders::_2));
    }
}

void Session::wirteAllToSocket(const std::string buf)
{
    sendQueue.emplace(std::make_shared<MsgNode>(buf.c_str(), buf.length()));
    if (sendPending_)
    {
        return;
    }
    sock_->async_send(boost::asio::buffer(buf),
                      std::bind(&Session::writeAllCallBack, this,
                                std::placeholders::_1, std::placeholders::_2));
    sendPending_ = true;
}

void Session::ReadFromSocket()
{
    if (_recv_pending)
    {
        return;
    }
    _recv_node = std::make_shared<MsgNode>(RECVSIZE);
    sock_->async_read_some(boost::asio::buffer(_recv_node->msg_, _recv_node->totalLen_),
                           std::bind(&Session::ReadCallBack, this, std::placeholders::_1, std::placeholders::_2));

    _recv_pending = true;
}

void Session::ReadCallBack(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
    _recv_node->curLen_ += bytes_transferred;
    if (_recv_node->curLen_ < _recv_node->totalLen_)
    {
        sock_->async_read_some(boost::asio::buffer(_recv_node->msg_ + _recv_node->curLen_, _recv_node->totalLen_ - _recv_node->curLen_),
                               std::bind(&Session::ReadCallBack, this, std::placeholders::_1, std::placeholders::_2));
        return;
    }

    _recv_pending = false;
    _recv_node = nullptr;
}

void Session::ReadAllFromSocket()
{
    if (_recv_pending)
    {
        return;
    }

    _recv_node = std::make_shared<MsgNode>(RECVSIZE);
    sock_->async_receive(boost::asio::buffer(_recv_node->msg_, _recv_node->totalLen_),
                         std::bind(&Session::ReadAllCallBack, this, std::placeholders::_1, std::placeholders::_2));

    _recv_pending = true;
}

void Session::ReadAllCallBack(const boost::system::error_code &ec, std::size_t bytes_transferred)
{
    _recv_node->curLen_ += bytes_transferred;
    _recv_node = nullptr;
    _recv_pending = false;
}
