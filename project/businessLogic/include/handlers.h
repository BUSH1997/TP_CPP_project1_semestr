#ifndef TEST_DB3_HANDLERS_H
#define TEST_DB3_HANDLERS_H

#include <string>

#include "parser.h"
#include "handlers.h"
#include "DBwrapper.h"
#include "dbExceptions.h"
#include "blExceptions.h"

#define FAILED 0
#define SUCCESS 1

#define MIN_PASSWORD_LENGTH 6
#define MIN_LOGIN_LENGTH 6

#define HOST "localhost"
#define USER "bushcast"
#define PASSWORD "Sergeykust_333"
#define DATABASE "test_base"

class HandlerHelper {
public:
    static std::string sha256(const std::string& str);
    static void validatePassword(const std::string& password);
    static void validateLogin(const std::string& login, bool isEmpty);
    static void completeJsonData(JsonData& jsonData, const std::vector<MessagesData>& messages);
    static void completeUsersData(UsersData& userData, const JsonData& jsonData);
    static void completeMessagesData(MessagesData& messageData, const JsonData& jsonData);
    static void writeLogger(const std::string& helper,const std::string& error);
};

class Handler {
public:
    virtual bool canHandle(const JsonData& jsonData) = 0;

    virtual JsonData handle(const JsonData& jsonData) = 0;
};

class UserCreatorHandler : public Handler {
public:
    bool canHandle(const JsonData& jsonData) override;

    JsonData handle(const JsonData& jsonData) override;

};

class UserAuthorizerHandler : public Handler {
public:
    bool canHandle(const JsonData& jsonData) override;

    JsonData handle(const JsonData& jsonData) override;
};

class UserDeAuthorizerHandler : public Handler {
public:
    bool canHandle(const JsonData& jsonData) override;

    JsonData handle(const JsonData& jsonData) override;
};

class MessageControllerHandler : public Handler {
public:
    bool canHandle(const JsonData& jsonData) override;

    JsonData handle(const JsonData& jsonData) override;

};

class LoadArchiveHandler : public Handler {
public:
    bool canHandle(const JsonData& jsonData) override;

    JsonData handle(const JsonData& jsonData) override;

};

class StartChatHandler : public Handler {
public:
    bool canHandle(const JsonData& jsonData) override;

    JsonData handle(const JsonData& jsonData) override;

};

#endif //TEST_DB3_HANDLERS_H

