#include "gtest/gtest.h"
#include "server.h"

#include <boost/asio.hpp>

#define SERVICE_PORT 9000

TEST(ServerTests, ConstructorLValsue) {
    boost::asio::io_context io;
    ASSERT_NO_THROW(AsyncServer server(io, SERVICE_PORT));
};

//TEST(ServerTests, start)

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

