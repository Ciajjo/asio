#include "AsioIOServicePool.h"

AsioIOServicePool::AsioIOServicePool(std::size_t size)
    : _ioService(size), _works(size), _nextLOService(0)
{
    for (std::size_t i; i < size; ++i)
    {
        auto unptr = std::unique_ptr<Work>(new Work(_ioService[i]));
        _works[i] = std::move(unptr);
    }
    for (std::size_t i; i < _ioService.size(); ++i)
    {
        _threads.emplace_back([this, i]()
                              { _ioService[i].run(); });
    }
}

AsioIOServicePool::~AsioIOServicePool()
{
    std::cout << "AsioIOServicePool destruct!!!" << std::endl;
}

boost::asio::io_context &AsioIOServicePool::getIOService()
{
    auto &service = _ioService[_nextLOService++];
    if (_nextLOService == _ioService.size())
    {
        _nextLOService = 0;
    }
    return service;

    // TODO: 在此处插入 return 语句
}

void AsioIOServicePool::Stop()
{
    for (auto &work : _works)
    {
        work.reset();
    }

    for (auto &t : _threads)
    {
        t.join();
    }
}
