#ifndef BUSINESSLOGIC_JSONDATA_H
#define BUSINESSLOGIC_JSONDATA_H

#include <string>


struct JsonData {
public:
    std::string requestType; // "message" / "registration" / "authorization" / "update" / "loadArchive"
    int transmitterId;
    int receiverId;
    int messageId;
    std::string date;
    std::string text;
    std::string contentType; // "audio" / "text"
    std::string name;
    std::string surname;
    int age;
    std::string password;
    std::string login;
    int userId;
    std::string updateDate;
    std::string status;
    int pushServerPort;
    JsonData* messages;
    int messagesNumber;
    std::string chatType; //"public" / "private"
};

#endif //BUSINESSLOGIC_JSONDATA_H
