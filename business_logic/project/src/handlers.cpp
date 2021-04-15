#include "handlers.h"

#include <string>

bool UserCreatorHandler :: canHandle(const JsonData& jsonData) {
    std::string registration = "registration";

    return (jsonData.requestType == registration);
}

JsonData UserCreatorHandler :: handleMessage(const JsonData& jsonData) {
    MySQLManager bdManager("localhost", "bush", "********", "database");

    UsersTableData userData;
    userData.name = jsonData.name;
    userData.surname = jsonData.surname;
    userData.age = jsonData.age;
    userData.login = jsonData.login;
    userData.password = jsonData.password;

    UserTableModule userTable;

    userTable.creatUser(bdManager.connection, userData);

    int userId = userTable.lastUserIdSelect(bdManager.connection);

    MySQLManager::closeConnection(bdManager.connection);

    JsonData jsonDataNew = jsonData;
    jsonDataNew.userId = userId;

    return jsonDataNew;
}


bool UserAuthorizerHandler :: canHandle(const JsonData& jsonData) {
    std::string authorization = "authorization";

    return (jsonData.requestType == authorization);
}

JsonData UserAuthorizerHandler :: handleMessage(const JsonData& jsonData) {
    MySQLManager bdManager("localhost", "bush", "********", "database");
    UserTableModule userTable;
    UsersTableData userData = userTable.getUserById(bdManager.connection, jsonData.userId);

    JsonData jsonDataNew = jsonData;

    if (jsonData.password != userData.password || jsonData.login != userData.login) {
        jsonDataNew.status = "declined";
    } else {
        jsonDataNew.status = "confirmed";
        jsonDataNew.pushServerPort = 5000;
    }

    MySQLManager::closeConnection(bdManager.connection);
    return jsonDataNew;
}

JsonData OldMessagesUpdaterHandler :: handleMessage(const JsonData& jsonData) {
    MySQLManager bdManager("localhost", "bush", "********", "database");
    MessagesTableModule messageTable;
    NewMessages messages = messageTable.selectNewMessagesByDate(bdManager.connection, jsonData.date, jsonData.receiverId);

    JsonData jsonDataNew = jsonData;

    completeJsonData(jsonDataNew, messages);
    MySQLManager::closeConnection(bdManager.connection);
    return jsonDataNew;
}

bool MessageControllerHandler :: canHandle(const JsonData& jsonData) {
    std::string message = "message";

    return (jsonData.requestType == message);
}

JsonData MessageControllerHandler :: handleMessage(const JsonData& jsonData) {
    MySQLManager bdManager("localhost", "bush", "********", "database");

    MessagesTableData messageData;
    messageData.transmitterId = jsonData.transmitterId;
    messageData.receiverId = jsonData.receiverId;
    messageData.date = jsonData.date;
    messageData.chatType = jsonData.chatType;
    messageData.text = jsonData.text;
    messageData.contentType = jsonData.contentType;

    MessagesTableModule messageTable;

    messageTable.putMessage(bdManager.connection, messageData);

    JsonData jsonDataNew = jsonData;

    MySQLManager::closeConnection(bdManager.connection);
    return jsonDataNew;
}


bool MessageUpdaterHandler :: canHandle(const JsonData& jsonData) {
    std::string update = "update";

    return (jsonData.requestType == update);
}

JsonData MessageUpdaterHandler :: handleMessage(const JsonData& jsonData) {
    MySQLManager bdManager("localhost", "bush", "********", "database");

    MessagesTableModule messageTable;
    NewMessages messages = messageTable.selectNewMessagesByDate(bdManager.connection, jsonData.updateDate, jsonData.receiverId);

    JsonData jsonDataNew = jsonData;

    completeJsonData(jsonDataNew, messages);
    MySQLManager::closeConnection(bdManager.connection);
    return jsonDataNew;
}


bool LoadArchiveHandler :: canHandle(const JsonData& jsonData) {
    std::string loadArchive = "loadArchive";

    return (jsonData.requestType == loadArchive);
}

JsonData LoadArchiveHandler :: handleMessage(const JsonData& jsonData) {
    MySQLManager bdManager("localhost", "bush", "********", "database");
    MessagesTableModule messageTable;

    NewMessages messages = messageTable.selectArchiveMessages(bdManager.connection, jsonData.receiverId);

    JsonData jsonDataNew = jsonData;

    completeJsonData(jsonDataNew, messages);
    MySQLManager::closeConnection(bdManager.connection);
    return jsonDataNew;
}




