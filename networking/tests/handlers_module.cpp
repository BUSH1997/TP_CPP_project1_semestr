#include <memory>
#include "gtest/gtest.h"
#include "handler.h"
#include "handlers_chain.h"

TEST(HandlerTest, GetInstance) {
    std::shared_ptr<HandlersChain> instance1 = HandlersChain::GetInstance();
    std::shared_ptr<HandlersChain> instance2 = HandlersChain::GetInstance();
    ASSERT_EQ(instance1, instance2);
}

TEST(HandlerTest, GetIterator) {
    std::shared_ptr<HandlersChain> instance = HandlersChain::GetInstance();

    auto it_beg = instance->GetBeg();
    auto it_end = instance->GetEnd();
    ASSERT_EQ(++it_beg, it_end);
}

TEST(HandlerTest, PushHandler) {
    std::shared_ptr<HandlersChain> instance = HandlersChain::GetInstance();

    Handler handler;
    ASSERT_NO_THROW(instance->PushHandler(handler));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

