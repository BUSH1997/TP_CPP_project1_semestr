#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

#define SERVICE_PORT 9000

class AsyncServer {
public:
    explicit AsyncServer(boost::asio::io_service &_io_service, int port);

    void Dispatch();

private:
    void run();

    void onAccept();

    void startAccept();

    boost::asio::io_service &m_io_service;
    boost::asio::ip::tcp::acceptor m_tcp_acceptor;
};

#endif //SERVER_H
