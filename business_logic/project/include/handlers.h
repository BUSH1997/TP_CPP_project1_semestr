#ifndef BUSINESSLOGIC_HANDLERS_H
#define BUSINESSLOGIC_HANDLERS_H

#include <string>
#include "jsonData.h"
#include "wrapperBD.h"

static void completeJsonData(JsonData& jsonDataNew , const NewMessages& messages) {};

class Handler {
public:
    virtual bool canHandle(const JsonData& jsonData) ;

    virtual JsonData handleMessage(const JsonData& jsonData);
};


class UserCreatorHandler : public Handler {
public:
    bool canHandle(const JsonData& jsonData) override;

    JsonData handleMessage(const JsonData& jsonData) override;

};

class UserAuthorizerHandler : public Handler {
public:
    bool canHandle(const JsonData& jsonData) override;

    JsonData handleMessage(const JsonData& jsonData) override;
};

class OldMessagesUpdaterHandler : public Handler {
public:
    JsonData handleMessage(const JsonData& jsonData) override;

    static void completeJsonData(JsonData& jsonDataNew , const NewMessages& messages) {};
};


class MessageControllerHandler : public Handler {
public:
    bool canHandle(const JsonData& jsonData) override;

    JsonData handleMessage(const JsonData& jsonData) override;

};

class MessageUpdaterHandler : public Handler {
public:
    bool canHandle(const JsonData& jsonData) override;

    JsonData handleMessage(const JsonData& jsonData) override;
};

class LoadArchiveHandler : public Handler {
public:
    bool canHandle(const JsonData& jsonData) override;

    JsonData handleMessage(const JsonData& jsonData) override;

};

#endif //BUSINESSLOGIC_HANDLERS_H
