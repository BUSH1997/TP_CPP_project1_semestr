#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <array>
#include <memory>

#define BUFFER_SIZE 4096

class Message {

};

class Connection : public std::enable_shared_from_this<Connection> {
public:
    explicit Connection(boost::asio::io_service &io_service_);

    inline boost::asio::ip::tcp::socket &GetSocket() { return m_socket; }

    void GetRequest(); // Это будет колбек для передачи полученного буффера в цепь хендлеров

    void SendResponse(); // Этот колбек будем дергать, чтобы отправить сообщение

private:
    boost::asio::ip::tcp::socket m_socket;
    boost::array<char, BUFFER_SIZE> m_msg_buffer;
    Message request;
    Message response;
};

#endif //CONNECTION_H
