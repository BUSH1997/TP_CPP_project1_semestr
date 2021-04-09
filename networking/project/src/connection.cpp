#include "connection.h"

Connection::Connection(boost::asio::io_context &io_context) : socket(io_context) {}
Connection::Connection(boost::asio::io_context &&io_context) : socket(io_context) {}

void Connection::GetRequest(const boost::system::error_code &ec) {
    if (ec) {
        throw std::exception();
    }
}

void Connection::SendResponse(boost::system::error_code &ec) {
    if (ec) {
        throw std::exception();
    }
}

std::string Connection::read() {
    return "placeholder";
}

void Connection::write(const std::string &message) {

}

void Connection::handleMessage(const std::string &message) {

}