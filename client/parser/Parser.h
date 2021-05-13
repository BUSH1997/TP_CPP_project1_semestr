#ifndef MAIN_CPP_PARSER_H
#define MAIN_CPP_PARSER_H

#include <string>

struct JsonData {
public:
    std::size_t jsonLength;
    std::string requestType; // "message" / "registration" / "authorization" / "update" / "loadArchive"
    std::size_t requestStatus;
    int transmitterId;
    int receiverId;
    std::string date;
    std::string text;
    std::string contentType; // "audio" / "text"
    std::string chatType; // "private" / "public"
    std::string name;
    std::string surname;
    std::string password;
    std::string login;
    int userId;
    std::string updateDate;
    std::string status;
    int pushServerPort;
    std::string path;
};

class JsonParser {
public:
    static JsonData jsonToJsonData(const std::string& json);
    static std::string jsonDataToJson(const JsonData& jsonData);
};

#endif //MAIN_CPP_PARSER_H
