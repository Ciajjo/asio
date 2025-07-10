#ifndef ASIOIOSERVICEPOOL_H
#define ASIOIOSERVICEPOOL_H

#include <boost/asio.hpp>
#include <vector>

#include "Singleon.hpp"

class AsioIOServicePool : public Singleon<AsioIOServicePool>
{
    friend Singleon<AsioIOServicePool>;

public:
    using IOService = boost::asio::io_context;
    using Work = boost::asio::io_context::work;
    using WorkPtr = std::unique_ptr<Work>;
    ~AsioIOServicePool();
    AsioIOServicePool(const AsioIOServicePool &) = delete;
    AsioIOServicePool &operator=(const AsioIOServicePool &) = delete;

    boost::asio::io_context &getIOService();
    void Stop();

private:
    AsioIOServicePool(std::size_t size = std::thread::hardware_concurrency());
    std::vector<IOService> _ioService;
    std::vector<WorkPtr> _works;
    std::vector<std::thread> _threads;
    std::size_t _nextLOService;
};

#endif // ASIOIOSERVICEPOOL_H