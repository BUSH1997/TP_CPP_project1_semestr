#include "gtest/gtest.h"
#include "handler.h"

TEST(HandlerTest, CanHandle) {
    Handler handler;
    ASSERT_FALSE(handler.canHandle("some message"));
}

TEST(HandlersPoolTest, GetInstance) {
    HandlersPool *pool1 = HandlersPool::GetInstance();
    HandlersPool *pool2 = HandlersPool::GetInstance();

    ASSERT_EQ(pool1, pool2);
};

TEST(HandlersPoolTest, SetNewHandler) {
    HandlersPool *pool = HandlersPool::GetInstance();
    ASSERT_EQ(pool->getBeg(), pool->getEnd());

    Handler new_handler;
    HandlersPool::SetNewHandler(new_handler);
    ASSERT_EQ(pool->getBeg() + 1, pool->getEnd());
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

