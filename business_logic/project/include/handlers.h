#ifndef BUSINESSLOGIC_HANDLERS_H
#define BUSINESSLOGIC_HANDLERS_H

#include <string>
#include "jsonData.h"
#include "wrapperBD.h"

static void completeJsonData(JsonData& jsonDataNew , const NewMessages& messages) {};

class Handler {
public:
    virtual bool canHandle() = 0;

    virtual JsonData handleMessage() = 0;
};


class UserCreatorHandler : public Handler {
    bool canHandle(const JsonData& jsonData);

    JsonData handleMessage(const JsonData& jsonData) ;

};

class UserAuthorizerHandler : public Handler {
    bool canHandle(const JsonData& jsonData);

    JsonData handleMessage(const JsonData& jsonData);

};

class OldMessagesUpdaterHandler : public Handler {
public:
    JsonData handleMessage(const JsonData& jsonData);

    static void completeJsonData(JsonData& jsonDataNew , const NewMessages& messages) {};
};


class MessageControllerHandler : public Handler {
    bool canHandle(const JsonData& jsonData);

    JsonData handleMessage(const JsonData& jsonData);

};

class MessageUpdaterHandler : public Handler {
    bool canHandle(const JsonData& jsonData);

    JsonData handleMessage(const JsonData& jsonData);
};

class LoadArchiveHandler : public Handler {
    bool canHandle(const JsonData& jsonData);

    JsonData handleMessage(const JsonData& jsonData);

};

#endif //BUSINESSLOGIC_HANDLERS_H
