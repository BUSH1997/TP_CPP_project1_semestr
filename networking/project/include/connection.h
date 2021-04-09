#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>
#include <array>
#include <memory>

#define BUFFER_SIZE 4096

class Connection : public std::enable_shared_from_this<Connection> {
public:
    explicit Connection(boost::asio::io_context &io_context);
    explicit Connection(boost::asio::io_context &&io_context);

    void GetRequest(const boost::system::error_code &ec);

    void SendResponse(boost::system::error_code &ec);

    boost::asio::ip::tcp::socket &GetSock() { return socket; }

private:
    std::string read();

    void write(const std::string &message);

    void handleMessage(const std::string &message);

    boost::asio::ip::tcp::socket socket;
    std::array<char, 4096> buffer;
    std::string request;
    std::string response;
};

#endif //CONNECTION_H
