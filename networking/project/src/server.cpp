#include <boost/asio.hpp>

#include "server.h"

AsyncServer::AsyncServer(boost::asio::io_service &_io_service, int port) : m_io_service(_io_service),
                                                                 m_tcp_acceptor(m_io_service) {
    boost::asio::ip::tcp::endpoint tcp_endpoint(boost::asio::ip::tcp::v4(), port);

}

void AsyncServer::Dispatch() {
    return;
}

void AsyncServer::startAccept() {
    return;
}

void AsyncServer::run() {return;}

void AsyncServer::onAccept() {
    return;
}