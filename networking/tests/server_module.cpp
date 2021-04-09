#include "gtest/gtest.h"
#include "server.h"

#include <boost/asio.hpp>

#define SERVICE_PORT 9000

TEST(ServerTests, ConstructorLValsue) {
    boost::asio::io_context io;
    AsyncServer server(io, SERVICE_PORT);
    EXPECT_TRUE(false);
};

TEST(ServerTests, ConstructorRValue) {
    AsyncServer server(boost::asio::io_context(), SERVICE_PORT);
    EXPECT_TRUE(false);
};

TEST(ServerTests, Dispatching) {
    boost::asio::io_context io;
    AsyncServer server(io, SERVICE_PORT);
    server.Dispatch();

    EXPECT_TRUE(false);
}

//TEST(ServerTests, start)

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

