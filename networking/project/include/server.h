#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

#include "connection.h"

#define SERVICE_PORT 9000

class AsyncServer {
public:
    explicit AsyncServer(boost::asio::io_service &_io_service, int port);

    explicit AsyncServer(boost::asio::io_service &&_io_service, int port);

    void Dispatch();

private:
    void run() { m_io_service.run(); };

    void onAccept(std::shared_ptr<Connection> new_connection, boost::system::error_code &ec);

    void startAccept();

    boost::asio::io_service &m_io_service;
    boost::asio::ip::tcp::acceptor m_tcp_acceptor;
};

#endif //SERVER_H
