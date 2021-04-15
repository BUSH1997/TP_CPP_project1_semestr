#include <memory>
#include "handlers_chain.h"

std::shared_ptr<HandlersChain> HandlersChain::GetInstance() {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_singleton == nullptr) {
        m_singleton = std::shared_ptr<HandlersChain>(new HandlersChain);
    }
    return m_singleton;
}