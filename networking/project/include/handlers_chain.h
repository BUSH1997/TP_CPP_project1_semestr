#ifndef HANDLERS_CHAIN_H
#define HANDLERS_CHAIN_H

#include <mutex>
#include <list>
#include <memory>
#include "handler.h"

class HandlersChain : public std::enable_shared_from_this<HandlersChain> {
public:
    static std::shared_ptr<HandlersChain> GetInstance();

    HandlersChain(const HandlersChain &handlers_chain) = delete;

    HandlersChain &operator=(const HandlersChain &handlers_chain) = delete;

    inline void PushHandler(const Handler &handler) { m_handlers_chain.push_back(handler); }

    inline std::list<Handler>::iterator GetBeg() { return m_handlers_chain.begin(); }

    inline std::list<Handler>::iterator GetEnd() { return m_handlers_chain.end(); }


private:
    HandlersChain() = default;

    std::list<Handler> m_handlers_chain;
    static inline std::shared_ptr<HandlersChain> m_singleton;
    static inline std::mutex m_mutex;

};

#endif //HANDLERS_CHAIN_H
