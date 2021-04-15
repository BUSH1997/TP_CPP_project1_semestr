

#include "gtest/gtest.h"

#include "wrapperBD.h"


TEST(wrapperBD, createConnectionSuccess) {
    const char* server, *user, *password, *database;
    server = "localhost";
    user = "bush";
    password = "Serj_333";
    database = "database";
    EXPECT_NO_THROW(MySQLManager manager(server, user, password, database));
}

TEST(wrapperBD, createConnectionNotSuccess) {
    const char* server, *user, *password, *database;
    server = "no_localhost";
    user = "bush";
    password = "Serj_333";
    database = "database";
    EXPECT_ANY_THROW(MySQLManager manager(server, user, password, database));
}

TEST(wrapperBD, closeConnectionSuccess) {
    const char* server, *user, *password, *database;
    server = "localhost";
    user = "bush";
    password = "Serj_333";
    database = "database";
    MySQLManager manager(server, user, password, database);
    EXPECT_NO_THROW(manager.connection);
}


TEST(wrapperBD, createUser) {
    const char* server, *user, *password, *database;
    server = "no_localhost";
    user = "bush";
    password = "Serj_333";
    database = "database";
    MySQLManager manager(server, user, password, database);

    UsersTableData userData;
    userData.name = "Sergey";
    userData.surname = "Kust";
    userData.password = "KKKK";
    userData.login = "BUSH";
    userData.age = 23;

    UserTableModule userTable;

    EXPECT_NO_THROW(userTable.creatUser(manager.connection, userData));
}

TEST(wrapperBD, createUserSuccess) {
    const char* server, *user, *password, *database;
    server = "no_localhost";
    user = "bush";
    password = "Serj_333";
    database = "database";
    MySQLManager manager(server, user, password, database);

    UsersTableData userData;
    userData.name = "Sergey";
    userData.surname = "Kust";
    userData.password = "KKKK";
    userData.login = "BUSH";
    userData.age = 23;

    UserTableModule userTable;

    userTable.creatUser(manager.connection, userData);

    int userId = userTable.lastUserIdSelect(manager.connection);
    UsersTableData receivedUserData = userTable.getUserById(manager.connection, userId);

    EXPECT_EQ(userData.name, receivedUserData.name);
    EXPECT_EQ(userData.surname, receivedUserData.surname);
    EXPECT_EQ(userData.password, receivedUserData.password);
    EXPECT_EQ(userData.login, receivedUserData.login);
    EXPECT_EQ(userData.age, receivedUserData.age);
}






int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


