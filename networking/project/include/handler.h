#ifndef HANDLER_H
#define HANDLER_H

#include <vector>
#include <string>

class Message {

};

class Handler {
public:
    virtual bool canHandle(const Message &request) { return false; }

    virtual Message handleMessage(const Message &request) {}
};

#endif //HANDLER_H
