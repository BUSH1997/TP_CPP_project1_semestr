#include "gtest/gtest.h"
#include "connection.h"

#include <boost/asio.hpp>

#define SERVICE_PORT 9000

TEST(ConnectionTests, Constructor) {
    boost::asio::io_context io;
    ASSERT_NO_THROW(Connection con(io));
};

TEST(ConnectionTests, GetSocket) {
    boost::asio::io_context io;
    Connection con(io);
    boost::asio::ip::tcp::socket &socket = con.GetSocket();
    ASSERT_TRUE(socket.available());
};

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

