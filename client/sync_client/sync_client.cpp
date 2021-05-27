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
using namespace boost::asio;
io_service service;

boost::recursive_mutex cs;

static char* parser(char* array) {
    std::list<char> list;
    auto result = array;
    list.push_back(*(array++));
    while (*(array++)) {
        if (*array == '{') {
            list.push_back(*array);
        }
        if (*array == '}') {
            list.pop_back();
            if (list.empty())
                break;
        }
    }
    *(++array) = '\0';
    return result;
}


struct talk_to_svr {
    talk_to_svr(const std::string & username) 
        : sock_(service), started_(true), username_(username) {}

    void connect(ip::tcp::endpoint ep) {
        sock_.connect(ep);
    }

    void authorization(JsonData& data) {
        data.requestType = "authorization";
        auto buff = JsonParser::jsonDataToJson(data);
        write(buff);
        read_answer();
        auto reply = JsonParser::jsonToJsonData(temp_);
        if (reply.errorDescription == 0) {
            std::cout << "authorization successful user # " << reply.users.back().userId << std::endl;
            return;
        } else {
            std::cout << "authorization error" << std::endl;
            return;
        }
    }


    void registration(JsonData& data) {
        data.requestType = "registration";
        auto buff = JsonParser::jsonDataToJson(data);
        write(buff);
        read_answer();
        auto reply = JsonParser::jsonToJsonData(temp_);
        if (reply.errorDescription == 0) {
            std::cout << "registration successful" << std::endl;
            return;
        } else {
            std::cout << "registration error" << std::endl;
            return;
        }
    }
    void write_message(JsonData& data) {
        data.requestType = "message";
        auto buff = JsonParser::jsonDataToJson(data);
        write(buff);
        read_answer();
        auto reply = JsonParser::jsonToJsonData(temp_);
        if (reply.errorDescription == 0) {
            std::cout << "message delivered" << std::endl;
            return;
        } else {
            std::cout << "message not delivered" << std::endl;
            return;
        }
    }


    void loop() {
        while ( started_) {
            boost::recursive_mutex::scoped_lock lk(cs);
            write_request();
            read_answer();

//            ----------------------------------
            // TODO here used receive handler

            auto receive = std::string(temp_);
            if (receive.find("ping") != 0) {
                std::cout << temp_ << std::endl;
            }
//            ----------------------------------

            int millis = 500;
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

private:
    size_t read_complete(const boost::system::error_code & err, size_t bytes) {
        if (err) return 1;
        return 0;
    }

    void write_request() {
        write("ping");
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 2048 };
    int already_read_;
    char buff_[max_msg];
    bool started_;
    std::string username_;

    char temp_[max_msg];
};

int main(int argc, char* argv[]) {
    UserData userData;
    userData.name = "Suleyman";
    userData.surname = "Kadyrov";
    userData.login = "JainaReaper228";
    userData.password = "12346";
    userData.userId = std::stoul(argv[1]);
    userData.updateDate = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    userData.status = "active";
    userData.avatarName = "";

    JsonData data;
    data.requestType = "authorization";
    data.requestStatus = 0;
    data.errorDescription = 1;
    data.users.push_back(userData);

    auto str = JsonParser::jsonDataToJson(data);

    ip::tcp::endpoint ep( ip::address::from_string("127.0.0.1"), 8001);
    talk_to_svr client(argv[1]);
    try {
        client.connect(ep);
        client.authorization(data);

        std::thread t(&talk_to_svr::loop, &client);
        t.detach();

//        std::thread t2(&talk_to_svr::read_loop, &client);
//        t2.detach();

//        std::this_thread::sleep_for(std::chrono::seconds(1));
//        data.requestType = "registration";
//        client.registration(data);

        std::this_thread::sleep_for(std::chrono::seconds(2));
        data.requestType = "message";
        MessageData mess;
        mess.transmitterId = data.users.back().userId;
        mess.receiverId = 2;
        mess.date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
        mess.text = "message from " + std::string(argv[1]) + " to " + std::to_string(mess.receiverId);
        mess.contentType = "text";
        mess.chatType = "private";
        mess.fileName = "";
        data.messages.push_back(mess);

        client.write_message(data);


//        client.authorization(data);

        while(true);


    }

    catch(boost::system::system_error & err) {
        std::cout << "client terminated " << client.username()
                  << ": " << err.what() << std::endl;
    }

}

