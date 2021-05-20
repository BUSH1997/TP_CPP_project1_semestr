#ifndef TEST_DB3_PARSER_H
#define TEST_DB3_PARSER_H

#include <string>
#include <vector>

struct UserData {
public:
    std::string name;
    std::string surname;
    std::string login;
    std::string password;
    std::size_t userId = 0;
    std::string updateDate;
    std::string status = "active";
    std::string avatarName;
};

struct MessageData {
    std::size_t transmitterId = 0;
    std::size_t receiverId = 0;
    std::string date;
    std::string text;
    std::string contentType; // "audio" / "text"
    std::string chatType; // "private" / "public"
    std::string fileName;
};

struct JsonData {
public:
    std::string requestType; // "message" / "registration" / "authorization" / "update" / "loadArchive"
    std::size_t requestStatus = 0;
    std::size_t errorDescription = 0;
    std::vector<UserData> users;
    std::vector<MessageData> messages;
};

class JsonParser {
public:
    static JsonData jsonToJsonData(const std::string& json);
    static std::string jsonDataToJson(const JsonData& jsonData);
};

#endif //TEST_DB3_PARSER_H

