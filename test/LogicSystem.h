#ifndef LOGICSYSTEM_H
#define LOGICSYSTEM_H

#include "Singleon.hpp"
#include "CSession.h"
#include "const.h"
#include "json.hpp"




#include <queue>
#include <thread>
#include <map>
#include <functional>

class CSession;  // 前置声明
class LogicNode;

using funCallBack = std::function<void(std::shared_ptr<CSession>, const short &msgId, const std::string &msgData)>;

class LogicSystem : public Singleon<LogicSystem>
{
    friend class Singleon<LogicSystem>;

public:
    ~LogicSystem();
    void postMsgToQue(std::shared_ptr<LogicNode> msg);

private:
    LogicSystem();
    void registerCallBack();
    void helloWorldCallback(std::shared_ptr<CSession>, const short &msgId, const std::string &msgData);
    void dealMsg();

    std::queue<std::shared_ptr<LogicNode>> que_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::thread workerThread_;
    bool stop_;
    std::map<short, funCallBack> funCallBacK_;
};

#endif // LOGICSYSTEM_H