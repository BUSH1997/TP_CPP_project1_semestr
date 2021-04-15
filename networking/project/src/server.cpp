#include <boost/asio.hpp>

#include "server.h"

AsyncServer::AsyncServer(boost::asio::io_service &io_service_, int port_)
        : m_io_service(io_service_), m_acceptor(boost::asio::ip::tcp::acceptor(m_io_service)) {
    /* Some logic */
}

void AsyncServer::Dispatch() {
    /* Some logic */
}

void AsyncServer::startAccept() {
    /* Some logic */
}

void AsyncServer::onAccept() {
    /* Some logic */
}
