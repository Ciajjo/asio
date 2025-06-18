#ifndef ENDPOINT_H
#define ENDPOINT_H

// class EndPoint
// {
// public:
//     EndPoint();
//     ~EndPoint();

// private:

// };

int clientEndPoint();
int serverEndPoint();
int createTcpSocket();
int createTcpAccept();
int bindAcceptSocket();
int connect();
int acceptclient();
int useConstBuffer();
int writeto(const boost::asio::ip::tcp::socket& sock);
int sendto();
int sendtoall();
#endif // ENDPOINT_H