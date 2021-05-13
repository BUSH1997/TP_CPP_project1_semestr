#ifndef TEST_DB3_DBEXCEPTIONS_H
#define TEST_DB3_DBEXCEPTIONS_H

#define NO_USER_FOUND 5
#define NO_MESSAGE_FOUND 6
#define CONNECTION_ERROR 7
#define QUERY_ERROR 8


#include <exception>

class MySQLDBExceptions: public std::exception
{
public:
    explicit MySQLDBExceptions(std::string error): m_error(std::move(error)) {}

    const char* what() const noexcept override{
        return m_error.c_str();
    }
    std::size_t errorCode;
private:
    std::string m_error;
};

class NoUserFound : public MySQLDBExceptions {
public:
    explicit NoUserFound(): MySQLDBExceptions("No user found.") {errorCode = NO_USER_FOUND;};
};

class NoMessageFound : public MySQLDBExceptions {
public:
    explicit NoMessageFound(): MySQLDBExceptions("No message found.") {errorCode = NO_MESSAGE_FOUND;};
};

class DBConnectionError : public MySQLDBExceptions {
public:
    explicit DBConnectionError(const std::string& error) : MySQLDBExceptions("Connection error" + error) {errorCode = CONNECTION_ERROR;};
};

class QueryError : public MySQLDBExceptions {
public:
    explicit QueryError(const std::string& error) : MySQLDBExceptions("Query error" + error) {errorCode = QUERY_ERROR;};
};

#endif //TEST_DB3_DBEXCEPTIONS_H
