#include "handlers.h"

#include <iomanip>
#include <openssl/sha.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

std::string HandlerHelper::sha256(const std::string& str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;

    for(unsigned char i : hash)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)i;
    }
    return ss.str();
}

void HandlerHelper::validatePassword(const std::string& password) {
    if (password.length() < MIN_PASSWORD_LENGTH || !std::any_of(password.begin(), password.end(), ::isdigit)
                                                || !std::any_of(password.begin(), password.end(), ::isalpha)
                                                || !std::none_of(password.begin(), password.end(), ::isspace)) {
        throw InvalidPassword();
    }
}

void HandlerHelper::validateLogin(const std::string& login, bool isEmpty) {
    if (login.length() < MIN_LOGIN_LENGTH || !std::none_of(login.begin(), login.end(), ::isspace)) {
        throw InvalidLogin();
    }
    if (!isEmpty) {
        throw LoginExists();
    }
}

void HandlerHelper::completeJsonData(JsonData& jsonData, const std::vector<MessagesData>& messages) {
    std::for_each(messages.begin(), messages.end(), [&jsonData](const MessagesData &message) {
        MessageData messageData;
        messageData.transmitterId = message.transmitterId;
        messageData.receiverId = message.receiverId;
        messageData.date = message.date;
        messageData.text = message.text;
        messageData.contentType = message.contentType;
        messageData.chatType = message.chatType;
        messageData.fileName = message.fileName;
        jsonData.messages.push_back(messageData);
    });
}

void HandlerHelper::completeUsersData(UsersData& userData, const JsonData& jsonData) {
    UserData user = jsonData.users[0];

    userData.name = user.name;
    userData.surname = user.surname;
    userData.login = user.login;
    userData.password = sha256(user.password);
    userData.updateDate = user.updateDate;
    userData.status = user.status;
    userData.avatarName = user.avatarName;
}

void HandlerHelper::completeMessagesData(MessagesData& messageData, const JsonData& jsonData) {
    MessageData message = jsonData.messages[0];

    messageData.transmitterId = message.transmitterId;
    messageData.receiverId = message.receiverId;
    messageData.date = message.date;
    messageData.chatType = message.chatType;
    messageData.text = message.text;
    messageData.contentType = message.contentType;
    messageData.fileName = message.fileName;
}

void HandlerHelper::writeLogger(const std::string& helper, const std::string& error) {
    std::ofstream logger("log/logger.log", std::ios::app);
    auto time = std::chrono::system_clock::now();
    std::time_t time_m = std::chrono::system_clock::to_time_t(time);
    logger << helper << " " << error << " " << std::ctime(&time_m) << "\n";
}

bool UserCreatorHandler::canHandle(const JsonData& jsonData) {
    std::string registration = "registration";

    return (jsonData.requestType == registration);
}

JsonData UserCreatorHandler::handle(const JsonData& jsonData) {
    JsonData jsonDataNew = jsonData;
    try {
        HandlerHelper::validatePassword(jsonData.users[0].password);

        connectionDetails details{HOST, USER, PASSWORD, DATABASE};
        MySQLManager dbManager(details);

        UserTable userTable(dbManager);

        HandlerHelper::validateLogin(jsonData.users[0].login, userTable.SELECT("login", jsonData.users[0].login).empty());

        UsersData userData;

        HandlerHelper::completeUsersData(userData, jsonData);

        userData.isAuthorized = 1;

        userTable.INSERT(userData);

        jsonDataNew.users[0].userId = userTable.SELECT("login", jsonData.users[0].login)[0].userId;

        jsonDataNew.requestStatus = SUCCESS;
    }
    catch (std::ifstream::failure& e) {
        std::string error = "Exception opening/reading/closing file";
        HandlerHelper::writeLogger("UserCreatorHandler", error);
    }

    catch (BusinessLogicExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
        HandlerHelper::writeLogger("UserCreatorHandler", e.what());
    }

    catch (MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
        HandlerHelper::writeLogger("UserCreatorHandler", e.what());
    }

    return jsonDataNew;
}

bool UserAuthorizerHandler::canHandle(const JsonData& jsonData) {
    std::string authorization = "authorization";

    return (jsonData.requestType == authorization);
}

JsonData UserAuthorizerHandler::handle(const JsonData& jsonData) {
    JsonData jsonDataNew = jsonData;
    try {
        connectionDetails details{HOST, USER, PASSWORD, DATABASE};
        MySQLManager dbManager(details);
        UserTable userTable(dbManager);
        UserData user = jsonData.users[0];

        UsersData userData = userTable.SELECT(user.userId);

       if (user.login != userData.login) {
            jsonDataNew.requestStatus = FAILED;
            jsonDataNew.errorDescription = INVALID_LOGIN;
        } else if (HandlerHelper::sha256(user.password) != userData.password) {
            jsonDataNew.requestStatus = FAILED;
            jsonDataNew.errorDescription = INVALID_PASSWORD;
        } else {
            jsonDataNew.requestStatus = SUCCESS;

            userTable.UPDATE(jsonData.users[0].userId, "is_authorized", "1");

            MessageTable messageTable(dbManager);

            std::vector<MessagesData> messages = messageTable.selectNewMessages(userData.userId, userData.updateDate);

            HandlerHelper::completeJsonData(jsonDataNew, messages);

            if (!messages.empty()) {
                userTable.UPDATE(messages[messages.size()  - 1].date, user.userId);
            }
        }
    }

    catch (std::ifstream::failure& e) {
        std::string error = "Exception opening/reading/closing file";
        HandlerHelper::writeLogger("UserAuthorizerHandler", error);
    }

    catch(NoUserFound& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = NO_USER_FOUND;
        HandlerHelper::writeLogger("UserAuthorizerHandler", e.what());
    }

    catch(MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
        HandlerHelper::writeLogger("UserAuthorizerHandler", e.what());
    }

    return jsonDataNew;
}

bool UserDeAuthorizerHandler::canHandle(const JsonData& jsonData) {
    std::string deAuthorization = "deAuthorization";

    return (jsonData.requestType == deAuthorization);
}

JsonData UserDeAuthorizerHandler::handle(const JsonData& jsonData) {
    JsonData jsonDataNew = jsonData;

    try {
        connectionDetails details{HOST, USER, PASSWORD, DATABASE};
        MySQLManager dbManager(details);
        UserTable userTable(dbManager);

        userTable.UPDATE(jsonData.users[0].userId, "is_authorized", "0");
    }
    catch (MySQLDBExceptions &e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
        HandlerHelper::writeLogger("UserDeAuthorizerHandler", e.what());
    }

    return jsonDataNew;
}

bool MessageControllerHandler::canHandle(const JsonData& jsonData) {
    std::string message = "message";

    return (jsonData.requestType == message);
}

JsonData MessageControllerHandler::handle(const JsonData& jsonData) {
    JsonData jsonDataNew = jsonData;

    try {
        connectionDetails details{HOST, USER, PASSWORD, DATABASE};
        MySQLManager dbManager(details);

        MessagesData messageData;
        HandlerHelper::completeMessagesData(messageData, jsonData);

        MessageTable messageTable(dbManager);
        messageTable.INSERT(messageData);

        UserTable userTable(dbManager);
        if (userTable.SELECT(jsonData.messages[0].receiverId).isAuthorized) {
            userTable.UPDATE(messageData.date, messageData.receiverId);
        };

        jsonDataNew.requestStatus = SUCCESS;
    }
    catch (std::ifstream::failure& e) {
        std::string error = "Exception opening/reading/closing file";
        HandlerHelper::writeLogger("MessageControllerHandler", error);
    }

    catch(MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
        HandlerHelper::writeLogger("MessageControllerHandler", e.what());
    }

    return jsonDataNew;
}

bool LoadArchiveHandler::canHandle(const JsonData& jsonData) {
    std::string loadArchive = "loadArchive";

    return (jsonData.requestType == loadArchive);
}

JsonData LoadArchiveHandler::handle(const JsonData& jsonData) {
    JsonData jsonDataNew = jsonData;

    try {
        connectionDetails details{HOST, USER, PASSWORD, DATABASE};
        MySQLManager dbManager(details);
        MessageTable messageTable(dbManager);

        std::vector<MessagesData> messages = messageTable.selectArchiveMessages(jsonData.messages[0].transmitterId,
                                                                 jsonData.messages[0].receiverId,
                                                                 jsonData.messages[0].date);

        HandlerHelper::completeJsonData(jsonDataNew, messages);

        messages = messageTable.selectArchiveMessages(jsonData.messages[0].receiverId,
                                                      jsonData.messages[0].transmitterId,
                                                      jsonData.messages[0].date);

        HandlerHelper::completeJsonData(jsonDataNew, messages);
    }
    catch(MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
        HandlerHelper::writeLogger("LoadArchiveHandler", e.what());
    }

    return jsonDataNew;
}

bool StartChatHandler::canHandle(const JsonData& jsonData) {
    std::string startChat = "startChat";

    return (jsonData.requestType == startChat);
}

JsonData StartChatHandler::handle(const JsonData &jsonData) {
    JsonData jsonDataNew = jsonData;
    try {
        connectionDetails details{HOST, USER, PASSWORD, DATABASE};
        MySQLManager dbManager(details);

        UserTable userTable(dbManager);

        std::vector<UsersData> usersData = userTable.SELECT("user_id", std::to_string(jsonData.users[0].userId));

        if (usersData[0].status == "deleted") {
            throw DeletedUser();
        }

        if (!usersData.empty()) {
            jsonDataNew.users[0].name = usersData[0].name;
            jsonDataNew.users[0].surname = usersData[0].surname;
            jsonDataNew.users[0].login = usersData[0].login;
        }
    }
    catch (MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
        HandlerHelper::writeLogger("StartChatHandler", e.what());
    }

    catch (BusinessLogicExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
        HandlerHelper::writeLogger("StartChatHandler", e.what());
    }

    return jsonDataNew;
}



