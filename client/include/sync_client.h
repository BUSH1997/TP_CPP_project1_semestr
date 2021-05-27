#ifndef SYNC_CLIENT_H
#define SYNC_CLIENT_H

#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <stdio.h>
#endif


#include "parser.h"

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <chrono>
#include <boost/noncopyable.hpp>
#include <QLineEdit>

#include <fstream>

using namespace boost::asio;

extern io_service service;
extern boost::recursive_mutex cs;
extern std::unique_ptr<QLineEdit> inputMessage;

class talk_to_svr {
public:
    talk_to_svr(const std::string & username)
        : sock_(service), started_(true), username_(username) {}



    void connect(ip::tcp::endpoint ep) {
        sock_.connect(ep);
    }

    JsonData authorization(JsonData& data) {
        data.requestType = "authorization";
        auto buff = JsonParser::jsonDataToJson(data);
//        ------------------------------------
        write(buff);

        std::this_thread::sleep_for(std::chrono::microseconds(10));

        read_answer();
//        ------------------------------------
        std::cout << "bruh" << std::endl;

        if (!temp_) {
            data.errorDescription = 5;
            return data;
        }

        auto reply = JsonParser::jsonToJsonData(temp_);
        std::cout << "bruh2" << std::endl;
        if (reply.errorDescription == 0) {
            std::cout << "authorization successful user # " << reply.users.back().userId << std::endl;
//            return;
        } else {
            std::cout << "authorization error" << std::endl;
//            return;
        }
        return reply;

    }

    JsonData registration(JsonData& data) {
        data.requestType = "registration";
        auto buff = JsonParser::jsonDataToJson(data);
        write(buff);
        std::this_thread::sleep_for(std::chrono::microseconds(10));
        read_answer();
        auto reply = JsonParser::jsonToJsonData(temp_);
        if (reply.errorDescription == 0) {
            std::cout << "registration successful" << std::endl;
//            return;
        } else {
            std::cout << "registration error" << std::endl;
//            return;
        }
        return reply;
    }

    JsonData write_message(JsonData& data) {
//        data.requestType = "message";
        auto buff = JsonParser::jsonDataToJson(data);
        write(buff);
        read_answer();
        auto reply = JsonParser::jsonToJsonData(temp_);
        if (reply.errorDescription == 0) {
            std::cout << "message delivered" << std::endl;
//            return;
        } else {
            std::cout << "message not delivered" << std::endl;
//            return;
        }
        return reply;
    }

    JsonData get_user(JsonData& data) {
        data.requestType = "new_dialog";
        auto buff = JsonParser::jsonDataToJson(data);

        write(buff);
        read_answer();

        auto reply = JsonParser::jsonToJsonData(temp_);
        if (reply.errorDescription == 0) {
            std::cout << "message delivered" << std::endl;
//            return;
        } else {
            std::cout << "message not delivered" << std::endl;
//            return;
        }
        return reply;
    }

    void loop() {
        while (started_) {

    boost::recursive_mutex::scoped_lock lk(cs);


            write_request();
            read_answer();

//            ----------------------------------
            // TODO here used receive handler

            if ((temp_[0] != '{') && (temp_[0] != 'p')) {
//                std::cout << temp_ << std::endl;
//                fileName = "test";

//                fileName = "test";
                boost::recursive_mutex::scoped_lock lk(cs);

                std::cout << fileName << std::endl;

                std::ofstream file(fileName, std::ios::out | std::ios::binary);

                 file.write(temp_, max_msg);

                 while(sock_.available()) {
                     sock_.read_some(buffer(temp_, max_msg));
                     file.write(temp_, max_msg);
                 }

                 file.close();

                 fileName = "temp";
//                 read_answer();
            } else {
                auto receive = std::string(temp_);
                if (receive.find("ping") != 0) {
                    std::cout << inputMessage << std::endl;
    //                std::cout << temp_ << std::endl;
                    inputMessage->setText(QString::fromStdString(temp_));
                };
            }




//            ----------------------------------

            int millis = 521;
            boost::this_thread::sleep(boost::posix_time::millisec(millis));
        }
    }

    std::string username() const { return username_; }

    void write(const std::string & msg) {
        sock_.write_some(buffer(msg));
    }

    void read_answer() {
        std::memset(temp_, 0, max_msg);

        try {
            sock_.read_some(buffer(temp_, max_msg));
        }
        catch(boost::system::system_error & err) {
            std::cout << "read_answer: " << err.what() << std::endl;
        }
    }

    void getInputMessageAddress(QLineEdit* other) {
        this->inputMessage = other;
    }

    bool isOpen() {
        return sock_.is_open();
    }

    void sendFile(const std::string filePath) {
        std::ifstream is(filePath, std::ios::in | std::ios::binary);
        char buf[2048];
        while (is.read(buf, sizeof(buf)).gcount() > 0) {
            sock_.write_some(buffer(buf, sizeof(buf)));
//            read_answer();
        }
//            sock_.write_some(buffer(buf, sizeof(buf)));
        is.close();
//        read_answer();
    }

    void receiveAudio() {

    }

private:
    size_t read_complete(const boost::system::error_code & err, size_t bytes) {
        if (err) return 1;
        return 0;
    }

    void write_request() {
        write("ping");
    }

public:
        std::string fileName;

private:

    ip::tcp::socket sock_;
    enum { max_msg = 65536 };
    int already_read_;
    char buff_[max_msg];
    bool started_;
    std::string username_;

    char temp_[max_msg];

    QLineEdit* inputMessage;


};

#endif // SYNC_CLIENT_H
