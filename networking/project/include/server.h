#ifndef SERVER_H
#define SERVER_H

#include <boost/asio.hpp>

#include "connection.h"

#define SERVICE_PORT 9000

class AsyncServer {
public:
    explicit AsyncServer(boost::asio::io_service &io_service, int port);

    void Dispatch();

private:
    void startAccept();

    inline void run() { m_io_service.run(); };

    void onAccept();

    boost::asio::io_service &m_io_service;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

#endif //SERVER_H
