#include "gtest/gtest.h"
#include "connection.h"

#include <boost/asio.hpp>

#define SERVICE_PORT 9000

TEST(ConnectionTests, Constructor) {
    boost::asio::io_context io;
    ASSERT_NO_THROW(Connection con(io));
};

class ConnectionFixture : public ::testing::Test {
protected:
    ConnectionFixture() : connection(boost::asio::io_context()) {}

    Connection connection;
};

TEST_F(ConnectionFixture, GetResponse) {
    auto err_type = boost::asio::error::eof;
    ASSERT_THROW(connection.GetRequest(err_type), std::exception);
}

TEST_F(ConnectionFixture, SendResponse) {
    auto err_type = boost::asio::error::eof;
    ASSERT_THROW(connection.GetRequest(err_type), std::exception);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

