#include "wrapperBD.h"

MySQLManager::MySQLManager(const char *server, const char *user, const char *password, const char *database): connection(initConnection(server, user, password, database)) {}

MYSQL* MySQLManager::initConnection(const char *server, const char *user, const char *password,
                                    const char *database) {
    MYSQL* connection = mysql_init(NULL);

    if (!mysql_real_connect(connection, server, user, password, database, 0, NULL, 0)) {}

    return connection;
}

void MySQLManager::closeConnection(MYSQL *connection) {
    mysql_close(connection);
    mysql_server_end ();
}


void UserTableModule::creatUser(MYSQL *connection, const UsersTableData &userData) {
    std::string query = "insert into users(name, surname, age, login, password) values('" + userData.name + "', '" + userData.surname + "', '" + std::to_string(userData.age) + "', '" + userData.login +  "', '" + userData.password + "');";
    MYSQL_RES* res = mysqlExecuteQuery(connection, query.c_str());
    mysql_free_result(res);
}

MYSQL_RES* BDTableModule::mysqlExecuteQuery(MYSQL *connection, const char *sql_query) {
    if (mysql_query(connection, sql_query)) {}

    return mysql_use_result(connection);
}
