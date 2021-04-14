#ifndef HANDLER_H
#define HANDLER_H

#include <vector>
#include <string>

class Handler {
public:
    virtual bool canHandle(const std::string &message) { return false; }

    virtual void handleMessage(const std::string &message) {}
};

#endif //HANDLER_H
