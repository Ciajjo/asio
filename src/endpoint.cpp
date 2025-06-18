#include "endpoint.h"

#include <boost/asio.hpp>
#include <iostream>

// EndPoint::EndPoint()
// {
// }

int clientEndPoint()
{
    std::string rawIpAddress = "127.0.0.1";
    unsigned short portNum = 3333;
    boost::system::error_code errorCode;
    boost::asio::ip::address ipAddress = boost::asio::ip::address::from_string(rawIpAddress, errorCode);
    if (errorCode.value() != 0)
    {
        std::cout << "Failed to parse the IP address. ErrorCode = " << errorCode.value() << " .Message is " << errorCode.message() << std::endl;
        return errorCode.value();
    }

    std::cout << "hello world" << std::endl;
    boost::asio::ip::tcp::endpoint ep(ipAddress, portNum);
}

int serverEndPoint()
{
    unsigned short portNum = 3333;
    boost::asio::ip::address ipAddress = boost::asio::ip::address_v4::any();
    boost::asio::ip::tcp::endpoint ep(ipAddress, portNum);
}

int createTcpSocket()
{
    boost::asio::io_context ioc;
    boost::asio::ip::tcp protocol = boost::asio::ip::tcp::v4();
    boost::asio::ip::tcp::socket sock(ioc);
    boost::system::error_code errorCode;
    sock.open(protocol, errorCode);
    if (errorCode.value())
    {
        std::cout << "Failed to parse the IP address. ErrorCode = " << errorCode.value() << " .Message is " << errorCode.message() << std::endl;
        return errorCode.value();
    }

    return 0;
}

int createTcpAccept()
{
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::acceptor acc(ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 3333));
    return 0;
}

int bindAcceptSocket()
{
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), 3333);
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::acceptor acc(ioc, ep.protocol());
    acc.bind(ep);
    return 0;
}

int connect()
{
    try
    {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 3333);
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
    }
    catch (boost::system::error_code &e)
    {
        std::cout << "Failed to parse the IP address. ErrorCode = " << e.value() << " .Message is " << e.message() << std::endl;
        return e.value();
    }

    return 0;
}

int acceptclient()
{
    // boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), 3333);
    // boost::asio::io_context ioc;
    // boost::asio::ip::tcp::socket sock(ioc);
    // boost::asio::ip::tcp::acceptor acc(ioc, ep.protocol());
    // acc.bind(ep);
    // acc.listen(30);
    // acc.accept(sock);
    // return 0;

    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address_v4::any(), 3333);
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::socket sock(ioc);
    boost::asio::ip::tcp::acceptor acc(ioc, ep.protocol());
    acc.bind(ep);
    acc.listen(30);
    acc.accept(sock);
}

int useConstBuffer()
{
    std::string buf = "hello world";
    boost::asio::const_buffer asioBuf(buf.c_str(), buf.length());
    std::vector<boost::asio::const_buffer> bufVec;
    bufVec.push_back(asioBuf);

    boost::asio::const_buffers_1 asiobuf = boost::asio::buffer("hello world");
    return 0;
}

int writeto(boost::asio::ip::tcp::socket &sock)
{
    std::string buf = "hello world";
    std::size_t len = 0;
    while (len != buf.length())
    {
        len += sock.write_some(boost::asio::buffer(buf.c_str() + len, buf.length() - len));
    }

    return 0;
}

int sendto()
{
    std::string ipAddress = "127.0.0.1";
    unsigned short portNum = 3333;
    try
    {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ipAddress), portNum);
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
        writeto(sock);
    }
    catch (boost::system::system_error &e)
    {
        std::cout << "Failed to parse the IP address. ErrorCode = " << e.code() << " .Message is " << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}

int sendtoall()
{
    std::string ipAddress = "127.0.0.1";
    unsigned short portNum = 3333;
    try
    {
        boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(ipAddress), portNum);
        boost::asio::io_context ioc;
        boost::asio::ip::tcp::socket sock(ioc, ep.protocol());
        sock.connect(ep);
        // writeto(sock);
        std::string buf = "hello world";
        sock.send(boost::asio::buffer(buf.c_str(), buf.length()));
    }
    catch (boost::system::system_error &e)
    {
        std::cout << "Failed to parse the IP address. ErrorCode = " << e.code() << " .Message is " << e.what() << std::endl;
        return e.code().value();
    }

    return 0;
}
