#ifndef ASIOTHREADPOOL_H
#define ASIOTHREADPOOL_H

#include "Singleon.hpp"

#include <boost/asio.hpp>

class AsioThreadPool : public Singleon<AsioThreadPool>
{
    friend class Singleon<AsioThreadPool>;

public:
    ~AsioThreadPool() {}
    AsioThreadPool(const AsioThreadPool &) = delete;
    AsioThreadPool &operator=(const AsioThreadPool &) = delete;
    boost::asio::io_context &getIOservice();
    void Stop();

private:
    AsioThreadPool(int threadNum = std::thread::hardware_concurrency());
    boost::asio::io_context _service;
    std::unique_ptr<boost::asio::io_context::work> _work;
    std::vector<std::thread> _threads;
};

#endif // ASIOTHREADPOOL_H