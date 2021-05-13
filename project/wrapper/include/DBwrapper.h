#ifndef TEST_DB3_DBWRAPPER_H
#define TEST_DB3_DBWRAPPER_H

#include <mysql/mysql.h>
#include <string>
#include <vector>
#include <exception>

struct connectionDetails {
    const char* server, *user, *password, *database;
};

typedef struct {
    std::size_t userId;
    std::string name;
    std::string surname;
    std::string login;
    std::string password;
    std::string avatarName;
    std::string updateDate;
    std::string status;
} UsersData;

typedef struct {
    std::size_t messageId;
    std::size_t transmitterId;
    std::size_t receiverId;
    std::string date;
    std::string text;
    std::string fileName;
    std::string contentType;
    std::string chatType;
} MessagesData;


class dbManager {
public:
    virtual void initConnection(struct connectionDetails mysqlDetails) = 0;
    virtual void closeConnection() = 0;

private:
    virtual void executeQuery(const char* sql_query) = 0;

};

class MySQLManager : dbManager {
public:
    MySQLManager() = default;
    explicit MySQLManager(struct connectionDetails mysqlDetails) : connection_m(mysql_connection_setup(mysqlDetails)) {}

    void initConnection(struct connectionDetails mysqlDetails) override;

    void executeQuery(const char* sqlQuery) override;

    ~MySQLManager() {
        closeConnection();
    }

    MYSQL* connection_m;

private:
    static MYSQL* mysql_connection_setup(struct connectionDetails);
    void closeConnection() override;
};

class UserTable {
public:
    explicit UserTable(MySQLManager& manager): mg(manager) {};
    void INSERT(const UsersData &userData);
    std::vector<UsersData> SELECT();
    std::vector<UsersData> SELECT(const std::string& selector, const std::string& value);
    UsersData SELECT(int id);
    int SELECT_LAST_USER_ID();
    MySQLManager& mg;
    void UPDATE(const std::string& date, int receiver_id);

private:
    static std::vector<UsersData> fillUserData( MYSQL_RES* res);
};

class MessageTable {
public:
    explicit MessageTable(MySQLManager& manager): mg(manager) {};
    void INSERT(const MessagesData &messageData);
    std::vector<MessagesData> SELECT();
    std::vector<MessagesData> SELECT(const std::string& selector, const std::string& value);
    MessagesData SELECT(int id);
    std::vector<MessagesData> selectArchiveMessages(int transmitterId, int receiverId, const std::string& date);
    std::vector<MessagesData> selectNewMessages(int receiverId, const std::string& date);
    MySQLManager& mg;

private:
    static std::vector<MessagesData> fillMessageData( MYSQL_RES* res);
};

#endif //TEST_DB3_DBWRAPPER_H

