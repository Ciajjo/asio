#include <boost/asio.hpp>
#include <iostream>

const int MAX_LEN = 1024;

int main()
{
    try
    {
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 10086);
        boost::asio::ip::tcp::socket sock(ioc);
        sock.connect(ep);

        char request[MAX_LEN];
        std::cin.getline(request, MAX_LEN);
        size_t len = strlen(request);
        boost::asio::write(sock, boost::asio::buffer(request, len));

        char res[MAX_LEN];
        size_t rlen = boost::asio::read(sock, boost::asio::buffer(res, len));
        std::cout.write(res, rlen);
        std::cout << "\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}