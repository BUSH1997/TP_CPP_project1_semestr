#ifndef TEST_DB3_BLEXCEPTIONS_H
#define TEST_DB3_BLEXCEPTIONS_H

#define INVALID_PASSWORD 1
#define INVALID_LOGIN 2
#define LOGIN_EXISTS 3
#define USER_DELETED 4

#include <exception>
#include <string>

class BusinessLogicExceptions: public std::exception
{
public:
    explicit BusinessLogicExceptions(std::string error): m_error(std::move(error)) {}

    const char* what() const noexcept override{
        return m_error.c_str();
    }
    std::size_t errorCode{};
private:
    std::string m_error;
};

class InvalidPassword : public BusinessLogicExceptions {
public:
    explicit InvalidPassword(): BusinessLogicExceptions("Invalid password!") {errorCode = INVALID_PASSWORD;};
};

class InvalidLogin : public BusinessLogicExceptions {
public:
    explicit InvalidLogin(): BusinessLogicExceptions("Invalid login!") {errorCode = INVALID_LOGIN;};
};

class LoginExists : public BusinessLogicExceptions {
public:
    explicit LoginExists(): BusinessLogicExceptions("Such login has already registered!") {errorCode = LOGIN_EXISTS;};
};

class DeletedUser : public BusinessLogicExceptions {
public:
    explicit DeletedUser(): BusinessLogicExceptions("User is deleted!") {errorCode = USER_DELETED;};
};

#endif //TEST_DB3_BLEXCEPTIONS_H

