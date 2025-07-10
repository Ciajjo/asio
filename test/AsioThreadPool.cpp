#include "AsioThreadPool.h"

AsioThreadPool::AsioThreadPool(int threadNum = std::thread::hardware_concurrency())
    : _work(new boost::asio::io_context::work(_service))
{
    _threads.emplace_back([this]()
                          { _service.run(); });
}

boost::asio::io_context &AsioThreadPool::getIOservice()
{
    return _service;
    // TODO: 在此处插入 return 语句
}

void AsioThreadPool::Stop()
{
    _work.reset();
    for (auto &t : _threads)
    {
        t.join();
    }
}