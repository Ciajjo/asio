#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include <queue>

const int RECVSIZE = 1024;

class MsgNode
{
public:
    MsgNode(const char *msg, int totalLen) : totalLen_(totalLen), curLen_(0)
    {
        msg_ = new char[totalLen];
        memcpy(msg_, msg, totalLen);
    }
    MsgNode(int totalLen) : totalLen_(totalLen), curLen_(0)
    {
        msg_ = new char[totalLen];
    }
    ~MsgNode()
    {
        delete[] msg_;
    }

    int totalLen_;
    int curLen_;
    char *msg_;
};

class Session
{
public:
    Session(std::shared_ptr<boost::asio::ip::tcp::socket> socket);
    void connect(const boost::asio::ip::tcp::endpoint &ep);
    void writeCallBackErr(const boost::system::error_code &ec, std::size_t byteTransferred, std::shared_ptr<MsgNode>);
    void wirteToSocketErr(const std::string buf);

    void writeCallBack(const boost::system::error_code &ec, std::size_t byteTransferred);
    void wirteToSocket(const std::string buf);

    void writeAllCallBack(const boost::system::error_code &ec, std::size_t byteTransferred);
    void wirteAllToSocket(const std::string buf);

    void ReadFromSocket();
    void ReadCallBack(const boost::system::error_code &ec, std::size_t bytes_transferred);
    void ReadAllFromSocket();
    void ReadAllCallBack(const boost::system::error_code &ec, std::size_t bytes_transferred);

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> sock_;
    std::shared_ptr<MsgNode> sendNode_;
    std::queue<std::shared_ptr<MsgNode>> sendQueue;
    bool sendPending_;

    std::shared_ptr<MsgNode> _recv_node;
    bool _recv_pending;
};

#endif // SESSION_H/