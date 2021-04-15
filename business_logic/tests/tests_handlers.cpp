

#include "gtest/gtest.h"

#include "wrapperBD.h"
#include "jsonParser.h"
#include "jsonData.h"
#include "handlers.h"


TEST(handlerBD, UserAuthorizerHandler) {
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

    UserTableModule userTable;

    userTable.creatUser(manager.connection, userData);

    int userId = userTable.lastUserIdSelect(manager.connection);
    UsersTableData receivedUserData = userTable.getUserById(manager.connection, userId);

    JsonData jsonData;
    jsonData.password = "KKKK";
    jsonData.login = "BUSH";
    jsonData.userId = userId;

    UserAuthorizerHandler handler;
    JsonData jsonDataReceive = handler.handleMessage(jsonData);
    EXPECT_EQ(jsonDataReceive.status, "confirmed");
}



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


