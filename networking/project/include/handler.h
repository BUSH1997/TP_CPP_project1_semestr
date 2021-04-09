#ifndef HANDLER_H
#define HANDLER_H

#include <vector>
#include <string>

class Handler {
public:
    virtual bool canHandle(const std::string &message) { return false; }

    virtual void handleMessage(const std::string &message) {}
};

class HandlersPool {
public:
    static HandlersPool *GetInstance();

    static void SetNewHandler(Handler &handler);

    static std::vector<Handler>::iterator getBeg() { return pool.begin(); }

    static std::vector<Handler>::iterator getEnd() { return pool.end(); }

private:
    HandlersPool() = default;

    HandlersPool(const HandlersPool &);

    HandlersPool &operator=(const HandlersPool &);

    inline static std::vector<Handler> pool;
    inline static HandlersPool *singleton;
};

#endif //HANDLER_H
