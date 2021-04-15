#ifndef BUSINESSLOGIC_JSONDATA_H
#define BUSINESSLOGIC_JSONDATA_H

#include <string>

class JsonData {
public:
    std::string requestType; // "message" / "registration" / "authorization" / "update" / "loadArchive"
    int transmitterId;
    int receiverId;
    int MessageId;
    std::string date;
    std::string text;
    std::string contentType; // "audio" / "text"
    std::string messageType; //"public" / "private"
    std::string name;
    std::string surname;
    int age;
    std::string password;
    std::string login;
    int userId;
    std::string updateDate;
    std::string status;
    int pushServerPort;
};


#endif //BUSINESSLOGIC_JSONDATA_H
