#include <boost/asio.hpp>
#include <iostream>
#include <memory>
#include <set>

const int MAX_LEN = 1024;
using sockPtr = std::shared_ptr<boost::asio::ip::tcp::socket>;
std::set<std::shared_ptr<std::thread>> threadSet;

void session(sockPtr sock)
{
    try
    {
        while (1)
        {

            char data[MAX_LEN];
            memset(data, '\0', MAX_LEN);
            boost::system::error_code err;

            size_t len = sock->read_some(boost::asio::buffer(data, MAX_LEN), err);
            if (err == boost::asio::error::eof)
            {
                std::cout << "conneaction closed by peer" << std::endl;
                break;
            }
            else if (err)
            {
                throw boost::system::system_error(err);
            }

            std::cout << "revice from " << sock->remote_endpoint().address().to_string() << std::endl;
            std::cout << "revice message is " << data << std::endl;

            boost::asio::write(*sock, boost::asio::buffer(data, len));
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void server(boost::asio::io_context &ioc, unsigned short port)
{
    boost::asio::ip::tcp::acceptor acc(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port));
    while (1)
    {
        sockPtr sock(std::make_shared<boost::asio::ip::tcp::socket>(ioc));
        acc.accept(*sock);
        auto it = std::make_shared<std::thread>(session, sock);
        threadSet.emplace(it);
    }
}

int main()
{
    try
    {
        boost::asio::io_context ioc;
        server(ioc, 10086);
        for (auto &t : threadSet)
        {
            t->join();
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
