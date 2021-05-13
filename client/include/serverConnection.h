#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

class ServerConnection: public boost::noncopyable {
private:
    enum { maxLength = 1024 };
    boost::asio::io_service io_service;
    tcp::socket s;
    tcp::resolver resolver;
    std::string ip = "127.0.0.1";
    std::string port = "8000";

    size_t request_length;

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
//            std::cout << request_length << std::endl;
            boost::asio::write(s, boost::asio::buffer(request, request_length));

            char reply[maxLength];
            size_t reply_length = boost::asio::read(s,
                boost::asio::buffer(reply, request_length));
//            std::cout << "Reply is: ";
//            std::cout.write(reply, request_length);
            std::cout << std::endl;

            return std::string(reply, message.length());
    }

    void write() {
            boost::asio::connect(s, resolver.resolve({ip, port}));

            std::cout << "Enter message: ";
            char request[maxLength];
            std::string message = "safdaf";
            std::copy(message.begin(), message.end(), request);
            request_length = std::strlen(request);
            boost::asio::write(s, boost::asio::buffer(request, request_length));

//            char reply[maxLength];
//            size_t reply_length = boost::asio::read(s,
//                boost::asio::buffer(reply, request_length));
//            std::cout << "Reply is: ";
//            std::cout.write(reply, reply_length);
//            std::cout << std::endl;


//        s.close();
    }

    void read() {
////        start();


////        char reply[maxLength];
////        size_t reply_length = boost::asio::read(s,
////            boost::asio::buffer(reply, request_length));
////        std::cout << reply << std::endl;

//        if (s.available() == 0) {
//            std::cout << "nothing to read" << std::endl;
//            return;
//        }

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
