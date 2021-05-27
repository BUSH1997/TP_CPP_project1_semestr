#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <iostream>

#include <QFile>

using boost::asio::ip::tcp;

class ServerConnection: public boost::noncopyable {
private:
    enum { maxLength = 2048 };
    boost::asio::io_service io_service;
    tcp::socket s;
    tcp::resolver resolver;
    std::string ip = "127.0.0.1";
    std::string port = "8000";

    size_t request_length;

    size_t bufferLength;

public:
    ServerConnection(): s(io_service), resolver(io_service) {
        std::cout << "ServerConnection()" << std::endl;
    }

    ~ServerConnection() {
        s.close();
    }

public:
    std::string echoWriteRead(std::string message) {
        if (!s.is_open()) {
            std::cout << "socket not open" << std::endl;
            return "";
        }

        char request[maxLength];
        std::copy(message.begin(), message.end(), request);
        size_t request_length = std::strlen(request);
        boost::asio::write(s, boost::asio::buffer(request, request_length));

        char reply[maxLength];
        size_t reply_length = boost::asio::read(s,
            boost::asio::buffer(reply, request_length));
        std::cout << std::endl;

        return std::string(reply, message.length());
    }

    std::string echoWriteReadFile(std::string size, std::string fileStr) {
        char buff[maxLength];
        std::copy(size.begin(), size.end(), buff);
        size_t buffLength = std::strlen(buff);
        boost::asio::write(s, boost::asio::buffer(buff, buffLength));

        char reply[maxLength];
        size_t replyLength = boost::asio::read(s,
            boost::asio::buffer(reply, buffLength));
        std::cout << std::endl;

        auto fileSize = std::stoll(size);
        char fileBuff[fileSize];
        std::copy(fileStr.begin(), fileStr.end(), fileBuff);
        boost::asio::write(s, boost::asio::buffer(fileBuff, fileSize));

        char fileReply[fileSize];
        size_t fileReplyLength = boost::asio::read(s,
            boost::asio::buffer(fileReply, fileSize));

        QFile r("audio/temp");
        r.open(QIODevice::WriteOnly);
        r.write(fileReply, fileSize);
        r.close();

        return std::string(fileReply, fileSize);
    }


    void writeFile(std::string size, std::string fileStr) {
        char buff[maxLength];
        std::copy(size.begin(), size.end(), buff);
        size_t buffLength = std::strlen(buff);
        boost::asio::write(s, boost::asio::buffer(buff, buffLength));

        char reply[maxLength];
        size_t replyLength = boost::asio::read(s,
            boost::asio::buffer(reply, buffLength));
        std::cout << std::endl;

        auto fileSize = std::stoll(size);
        char fileBuff[fileSize];
        std::copy(fileStr.begin(), fileStr.end(), fileBuff);
        boost::asio::write(s, boost::asio::buffer(fileBuff, fileSize));

        char fileReply[fileSize];
        size_t fileReplyLength = boost::asio::read(s,
            boost::asio::buffer(fileReply, fileSize));

        QFile r("temp");
        r.open(QIODevice::WriteOnly);
        r.write(fileReply, fileSize);
        r.close();
    }



    void write() {
        boost::asio::connect(s, resolver.resolve({ip, port}));

        std::cout << "Enter message: ";
        char request[maxLength];
        std::string message = "safdaf";
        std::copy(message.begin(), message.end(), request);
        request_length = std::strlen(request);
        boost::asio::write(s, boost::asio::buffer(request, request_length));

    }

    void write(std::string& message) {
        boost::asio::connect(s, resolver.resolve({ip, port}));
        char buffer[maxLength];
        std::copy(message.begin(), message.end(), buffer);
        bufferLength = std::strlen(buffer);
        boost::asio::write(s, boost::asio::buffer(buffer, bufferLength));
    }

    void read() {

        char reply[maxLength];
        size_t reply_length = boost::asio::read(s,
            boost::asio::buffer(reply, request_length));
        std::cout << "Reply is: ";
        std::cout.write(reply, reply_length);
        std::cout << std::endl;


    }

    bool start() {
        try {
            boost::asio::connect(s, resolver.resolve({ip, port}));
        }
        catch (...) {
            std::cout << "socket not open" << std::endl;
            s.close();
        }
        return !s.is_open();
    }

    void stop() {
        if (!s.is_open()) {
            return;
        }
        s.close();
    }
};


#endif // SERVERCONNECTION_H
