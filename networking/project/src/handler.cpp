#include "handler.h"

HandlersPool * HandlersPool::GetInstance() {
    if (singleton == nullptr) {
        singleton = new HandlersPool;
    }
    return singleton;
}

void HandlersPool::SetNewHandler(Handler &handler) {
    pool.push_back(handler);
}