#ifndef BUSINESSLOGIC_WRAPPERBD_H
#define BUSINESSLOGIC_WRAPPERBD_H

#include <iostream>
#include <mysql/mysql.h>

typedef struct {
    int userId;
    std::string name;
    std::string surname;
    int age;
    std::string login;
    std::string password;
    std::string updateDate;
    bool needUpdate;
} UsersTableData;

typedef struct {
    int messageId;
    int transmitterId;
    int receiverId;
    std::string date;
    std::string text;
    std::string path;
    std::string contentType;
    std::string chatType;
} MessagesTableData;

typedef struct {
    MessagesTableData* messages;
    int messagesNumber;
} NewMessages;


class MySQLManager {
public:
    MySQLManager() = default;
    ~MySQLManager() = default;
    MySQLManager(const char* server, const char* user, const char* password, const char* database);

    static void closeConnection(MYSQL* connection);

    MYSQL* connection;

private:
    static MYSQL* initConnection(const char *server, const char *user, const char *password,
                                 const char *database);

};

class BDTableModule {
protected:
    static MYSQL_RES* mysqlExecuteQuery(MYSQL* connection, const char* sqlQuery);
};


class UserTableModule : public BDTableModule {
public:
     void creatUser(MYSQL* connection, const UsersTableData& userData);

     int lastUserIdSelect(MYSQL* connection) {};

     UsersTableData getUserById(MYSQL* connection, int userId) {};
};


class MessagesTableModule : public BDTableModule {
public:
     NewMessages selectNewMessagesByDate(MYSQL* connection, const std::string& date, const int receiverId) {};

     void putMessage(MYSQL* connection, const MessagesTableData& messageData) {};

     NewMessages selectArchiveMessages(MYSQL* connection, const int receiverId) {};
};



#endif //BUSINESSLOGIC_WRAPPERBD_H
