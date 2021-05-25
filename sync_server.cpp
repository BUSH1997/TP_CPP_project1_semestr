#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <map>
#include <fstream>

#include "DBwrapper.h"
#include "dbExceptions.h"
#include "handlers.h"
#include "blExceptions.h"
#include "parser.h"


static std::string parser(std::string data) {
    std::list<char> list;

    if (data == "ping") {
        return data;
    }

    std::string result = data;

    if (data.find("\"requestType\": \"file\",") != std::string::npos) {
        auto left = data.find("\"text\"") + 9;
        auto right = data.find("\"contentType\"") - 15;

        auto text = std::string(data, left, right - left);
        data.erase(left, right - left);

        std::ofstream file("test");
        file << text;
        file.close();
    }


    int i = 0;
    for (auto it = data.begin(); it != data.end(); ++it, ++i) {

        if (*it == '{') {
            list.push_back('1');
        }
        if (*it == '}') {
            list.pop_back();
            if (list.empty()) {
                result = std::string(data, 0, i + 1);
                break;
            }
        }
    }

    return result;
}


using namespace boost::asio;
using namespace boost::posix_time;

io_service service;

struct talk_to_client;
typedef boost::shared_ptr<talk_to_client> client_ptr;

typedef std::vector<client_ptr> array;
array clients;

std::map<std::size_t, client_ptr> clientsMap;

boost::recursive_mutex cs;

struct talk_to_client : boost::enable_shared_from_this<talk_to_client> {
    talk_to_client() 
        : sock_(service), started_(false), already_read_(0) {
        last_ping = microsec_clock::local_time();
    }
    std::string username() const { return username_; }

    void answer_to_client() {
        try {
            read_request();
            process_request();
        } catch ( boost::system::system_error&) {
            stop();
        }
        if ( timed_out()) {
            stop();

        if (userId) {
            JsonData temp;
            temp.users.emplace_back(UserData());
            temp.users.back().userId = userId;

            std::cout << JsonParser::jsonDataToJson(temp) << std::endl;
            UserDeAuthorizerHandler handler;
            JsonData newData = handler.handle(temp);
        }

            clientsMap.erase(userId);
            std::cout << "stopping user # " << userId << " - no ping in time" << std::endl;
        }
    }

    ip::tcp::socket & sock() { return sock_; }
    bool timed_out() const {
        ptime now = microsec_clock::local_time();
        long long ms = (now - last_ping).total_milliseconds();
        return ms > 5000 ;
    }
    void stop() {
        boost::system::error_code err;
        sock_.close(err);
    }
private:
    void read_request() {
        std::memset(temp_, 0, max_msg);
        result.clear();
        while (sock_.available()) {
            sock_.read_some(buffer(temp_, max_msg));
            result.append(temp_);
        }
    }

    void process_request() {
        if (result.empty()) {
            return;
        }
        last_ping = microsec_clock::local_time();
        std::string msg(parser(result));

        if (msg.find("ping") == 0) {
            if (new_message) {
                write(JsonParser::jsonDataToJson(messageJson));
                new_message = false;
                std::cout << "message to client " << userId << std:: endl;
            } else {
                write("ping");
            }
            return;
        }
        std::cout << "bruh2" << std::endl;
        std::cout << msg << std::endl;


        JsonData data = JsonParser::jsonToJsonData(msg);
        if (data.requestType == "authorization") {

            UserAuthorizerHandler handler;
            JsonData newData = handler.handle(data);

            auto string = JsonParser::jsonDataToJson(newData);
            std::cout << string << std::endl;

            userId = newData.users.back().userId;
            clientsMap[userId] = clients.back();

            std::cout << "user # " << userId << " logged in" << std::endl;


            auto reply = JsonParser::jsonDataToJson(newData);
            std::cout << reply << std::endl;
            write(reply);
            return;
        }

        if (data.requestType == "registration") {

            UserCreatorHandler handler;
            JsonData newData = handler.handle(data);

            auto string = JsonParser::jsonDataToJson(newData);
            std::cout << string << std::endl;

            userId = newData.users.back().userId;

            auto reply = JsonParser::jsonDataToJson(data);
            write(string);

            std::cout << "registration handler" << std::endl;
            return;
        }

        if (data.requestType == "file") {
            data.errorDescription = 0;
            auto reply = JsonParser::jsonDataToJson(data);
            write(reply);

            std::string temp(data.messages.back().text, 0, std::stoll(data.messages.back().fileName));
            std::cout << temp << std::endl;
            write("ping");
            return;
        }

        if (data.requestType == "message") {

            MessageControllerHandler handler;
            JsonData newData = handler.handle(data);

            auto reply = JsonParser::jsonDataToJson(newData);

            write(reply);

            std::cout << "message handler" << std::endl;

            message_to_client = JsonParser::jsonDataToJson(newData);

            if (!clientsMap[data.messages.back().receiverId]) {
                return;
            }
            clientsMap[data.messages.back().receiverId]->messageJson = data;
            clientsMap[data.messages.back().receiverId]->new_message = true;
            return;
        }

        if (data.requestType == "get_user") {

                StartChatHandler handler;

                JsonData newData = handler.handle(data);
            auto reply = JsonParser::jsonDataToJson(newData);
            std::cout << reply << std::endl;

            write(reply);

            return;

            }


        if (data.requestType == "deauthorize") {

            UserDeAuthorizerHandler handler;
            JsonData newData = handler.handle(data);

            auto reply = JsonParser::jsonDataToJson(newData);
            std::cout << reply << std::endl;

            std::cout << "deauthorize handler" << std::endl;
            return;
        }

        write("ping");

    }

    void write(const std::string & msg) {
        sock_.write_some(buffer(msg));
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 2048 };
    int already_read_;
    char buff_[max_msg];
    bool started_;
    std::string username_;
    ptime last_ping;

    char temp_[max_msg];
    std::size_t userId;
    bool new_message = false;
    std::string message_to_client;
    JsonData messageJson;

    long long fileSize = 0;
    bool fileReadFlag = false;
    long long fileSizeBuff = max_msg;


    std::string result;
};

[[noreturn]] void accept_thread() {

    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8001));

    while (true) {
        client_ptr new_( new talk_to_client);
        acceptor.accept(new_->sock());
        
        boost::recursive_mutex::scoped_lock lk(cs);
        clients.push_back(new_);
    }
}

[[noreturn]] void handle_clients_thread() {

    while (true) {
        boost::this_thread::sleep( millisec(1));
        boost::recursive_mutex::scoped_lock lk(cs);

        for ( array::iterator b = clients.begin(), e = clients.end(); b != e; ++b) 
            (*b)->answer_to_client();

        clients.erase(std::remove_if(clients.begin(), clients.end(), 
                   boost::bind(&talk_to_client::timed_out,_1)), clients.end());
    }
}

int main(int argc, char* argv[]) {
    boost::thread_group threads;
    threads.create_thread(accept_thread);
    threads.create_thread(handle_clients_thread);
    threads.join_all();
}

