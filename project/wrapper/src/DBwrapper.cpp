#include "DBwrapper.h"
#include "dbExceptions.h"

void MessageTable::INSERT(const MessagesData &messageData) {
    std::string ss = "insert into messages(transmitter_id, receiver_id, date_time, text, path, contentType, chatType) values('" + std::to_string(messageData.transmitterId) + "', '" + std::to_string(messageData.receiverId) + "', '" + messageData.date +  "', '" + messageData.text + "', '" + messageData.fileName + "', '" + messageData.contentType + "', '" + messageData.chatType + "');";
    mg.executeQuery(ss.c_str());
}

std::vector<MessagesData> MessageTable::SELECT() {
    std::string ss = "select * from messages;";
    mg.executeQuery(ss.c_str());
    MYSQL_RES* res = mysql_use_result(mg.connection_m);

    return fillMessageData(res);
}

std::vector<MessagesData> MessageTable::SELECT(const std::string& selector, const std::string& value) {
    std::string ss = "select * from messages where " + selector + "='" + value + "';";
    mg.executeQuery(ss.c_str());
    MYSQL_RES* res = mysql_use_result(mg.connection_m);

    return fillMessageData(res);
}

MessagesData MessageTable::SELECT(int id) {
    std::string ss = "select * from messages where id='" + std::to_string(id) + "';";
    mg.executeQuery(ss.c_str());
    MYSQL_RES* res = mysql_use_result(mg.connection_m);

    return fillMessageData(res)[0];
}

std::vector<MessagesData> MessageTable::selectArchiveMessages(int transmitterId, int receiverId, const std::string& date) {
    std::string ss = "select * from messages where transmitter_id='" + std::to_string(transmitterId) + "' AND receiver_id='" + std::to_string(receiverId) + "' AND date_time < '" + date + "' LIMIT 10;";
    mg.executeQuery(ss.c_str());
    MYSQL_RES* res = mysql_use_result(mg.connection_m);

    return fillMessageData(res);
}

std::vector<MessagesData> MessageTable::selectNewMessages(int receiverId, const std::string& date) {
    std::string ss = "select * from messages where receiver_id='" + std::to_string(receiverId) + "' AND date_time > '" + date + "';";
    mg.executeQuery(ss.c_str());
    MYSQL_RES* res = mysql_use_result(mg.connection_m);

    return fillMessageData(res);
}

std::vector<MessagesData> MessageTable::fillMessageData(MYSQL_RES* res) {
    MYSQL_ROW row;
    std::vector<MessagesData> messages;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        MessagesData message{  static_cast<size_t>(std::stoi(row[0])),
                             static_cast<size_t>(std::stoi(row[1])),
                               static_cast<size_t>(std::stoi(row[2])),
                                   row[3], row[4], row[5], row[6], row[7]};
        messages.push_back(message);
    }
    mysql_free_result(res);
    return messages;
}


void UserTable::INSERT(const UsersData &userData) {
std::string ss = "insert into users(name, surname, age, login, password, update_date, status) values('" + userData.name + "', '" + userData.surname + "', '" + std::to_string(userData.age) + "', '" + userData.login +  "', '" + userData.password + "', '" + userData.updateDate + "', '" + userData.status + "');";
   mg.executeQuery(ss.c_str());
}

std::vector<UsersData> UserTable::SELECT() {
    std::string ss = "select * from users;";
    mg.executeQuery(ss.c_str());
    MYSQL_RES* res = mysql_use_result(mg.connection_m);

    return fillUserData(res);
}

std::vector<UsersData> UserTable::SELECT(const std::string& selector, const std::string& value) {
    std::string ss = "select * from users where " + selector + "='" + value + "';";
    mg.executeQuery(ss.c_str());
    MYSQL_RES* res = mysql_use_result(mg.connection_m);

    return fillUserData(res);
}

UsersData UserTable::SELECT(int id) {
    std::string ss = "select * from users where user_id='" + std::to_string(id) + "';";
    mg.executeQuery(ss.c_str());
    MYSQL_RES* res = mysql_use_result(mg.connection_m);

    std::vector<UsersData> users = fillUserData(res);

    if (users.empty()) {
        throw NoUserFound();
    }

    return users[0];
}

int UserTable::SELECT_LAST_USER_ID() {
    std::string ss = "select * from users order by user_id desc limit 1;";
    mg.executeQuery(ss.c_str());
    MYSQL_RES* res = mysql_use_result(mg.connection_m);
    MYSQL_ROW row = mysql_fetch_row(res);
    int userId = std::stoi(row[0]);
    mysql_free_result(res);

    return userId;
}

void UserTable::UPDATE(const std::string& date, int receiver_id) {
    std::string ss = "update users set update_date = '" + date + "' where user_id = '" + std::to_string(receiver_id) + "';";
    mg.executeQuery(ss.c_str());
}

std::vector<UsersData> UserTable::fillUserData(MYSQL_RES* res) {
    MYSQL_ROW row;
    std::vector<UsersData> users;
    while ((row = mysql_fetch_row(res)) != nullptr) {
        UsersData user{static_cast<size_t>(std::stoi(row[0])), row[1], row[2],
                         static_cast<size_t>(std::stoi(row[3])) , row[4], row[5],  row[6], row[7]};
        users.push_back(user);
    }
    mysql_free_result(res);
    return users;
}


MYSQL* MySQLManager::mysql_connection_setup(struct connectionDetails mysqlDetails) {
    MYSQL* connection = mysql_init(nullptr);

    if (!mysql_real_connect(connection, mysqlDetails.server, mysqlDetails.user, mysqlDetails.password, mysqlDetails.database, 0, nullptr, 0)) {
        std::string error = mysql_error(connection);
        throw DBConnectionError(error);
    }

    return connection;
}

void MySQLManager::initConnection(struct connectionDetails mysqlDetails) {
    try {
        connection_m = mysql_connection_setup(mysqlDetails);
    }
    catch (MySQLDBExceptions& e) {
        initConnection(mysqlDetails);
    }
}

void MySQLManager::closeConnection() {
    mysql_close(connection_m);
    mysql_server_end();
}

void MySQLManager::executeQuery(const char* sqlQuery) {
    if (mysql_query(connection_m, sqlQuery)) {
        std::string error = mysql_error(connection_m);
        throw QueryError(error);
    }
}
