#include "handlers.h"

#include <iomanip>
#include <openssl/sha.h>
#include <algorithm>
#include <iostream>
#include <fstream>

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
                                                || !std::none_of(password.begin(), password.end(), ::ispunct)
                                                || !std::none_of(password.begin(), password.end(), ::isspace)) {
        throw InvalidPassword();
    }
}

void HandlerHelper::validateLogin(const std::string& login, bool isEmpty) {
    if (login.length() < MIN_LOGIN_LENGTH || !std::all_of(login.begin(), login.end(), ::isalnum)) {
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

        userTable.INSERT(userData);

        jsonDataNew.users[0].userId = userTable.SELECT("login", jsonData.users[0].login)[0].userId;

        if (jsonData.users[0].avatarSize != 0) {
            std::string sample = jsonData.users[0].avatarData;
            std::ofstream file("../img/" + jsonData.users[0].avatarName);
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            if (file.is_open()) {
                file << sample << std::endl;
            }
            file.close();
        }

        jsonDataNew.requestStatus = SUCCESS;
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "Exception opening/reading/closing file\n";
    }

    catch (BusinessLogicExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
    }

    catch (MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
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
        }
        else if (HandlerHelper::sha256(user.password) != userData.password) {
            jsonDataNew.requestStatus = FAILED;
            jsonDataNew.errorDescription = INVALID_PASSWORD;
        } else {
            jsonDataNew.requestStatus = SUCCESS;
            HandlerHelper::completeUsersData(userData, jsonDataNew);

           /* std::ifstream file("../img/" + userData.avatarName);
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            if (file.is_open()) {
                file >> jsonDataNew.users[0].avatarData;
            }
            file.close();*/

            MessageTable messageTable(dbManager);

            std::vector<MessagesData> messages = messageTable.selectNewMessages(user.userId, userData.updateDate);

            HandlerHelper::completeJsonData(jsonDataNew, messages);

            std::for_each(jsonDataNew.messages.begin(), jsonDataNew.messages.end(), [](MessageData& message) {
                if (message.contentType == "audio") {
                    std::ifstream file("../audio/" + message.fileName);
                    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
                    if (file.is_open()) {
                        file >> message.fileData;
                    }
                    file.close();
                }
            });
            if (!messages.empty()) {
                userTable.UPDATE(messages[messages.size()  - 1].date, user.userId);
            }
        }
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "Exception opening/reading/closing file\n";
    }

    catch(NoUserFound& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = NO_USER_FOUND;
    }

    catch(MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
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

        if (jsonData.messages[0].contentType == "audio") {
            std::string sample = jsonData.messages[0].fileData;
            std::ofstream file("../audio/" + jsonData.messages[0].fileName);
            file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            if (file.is_open()) {
                file << sample << std::endl;
            }
            file.close();
        }

        jsonDataNew.requestStatus = SUCCESS;
    }
    catch (std::ifstream::failure& e) {
        std::cerr << "Exception opening/reading/closing file\n";
    }

    catch(MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
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
                                                                 jsonData.users[0].updateDate);

        HandlerHelper::completeJsonData(jsonDataNew, messages);
    }
    catch(MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
    }

    return jsonDataNew;
}

bool UpdateDateHandler::canHandle(const JsonData& jsonData) {
    return false;
}

JsonData UpdateDateHandler::handle(const JsonData &jsonData) {
    JsonData jsonDataNew = jsonData;
    try {
        connectionDetails details{HOST, USER, PASSWORD, DATABASE};
        MySQLManager dbManager(details);

        UserTable userTable(dbManager);
        userTable.UPDATE(jsonData.messages[0].date, jsonData.messages[0].receiverId);
    }
    catch (MySQLDBExceptions& e) {
        std::cout << e.what() << std::endl;
        jsonDataNew.requestStatus = FAILED;
        jsonDataNew.errorDescription = e.errorCode;
    }
    return jsonDataNew;
}

