#ifndef APPLICATION_H
#define APPLICATION_H

#include "mainwidget.h"
#include "authorizwindow.h"
#include "registrationwindow.h"
#include "sync_client.h"

#include <memory>
#include <QFrame>
#include <QObject>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/thread.hpp>
#include <memory>

using namespace boost::asio;

namespace Ui {
class Frame;
}

namespace Ui {
class RegistrationWindow;
}

class Application: public QObject {
    Q_OBJECT
private:
    std::unique_ptr<MainWidget> mainWidget;
    std::unique_ptr<AuthorizWindow> authorizWindow;
    std::unique_ptr<RegistrationWindow> registrationWindow;

    std::map<int, QString> errorMap{
        {0, "Success"},
        {1, "Invalid Password"},
        {2, "Invalid Login"},
        {3, "Login already exist"},
        {4, "User was deleted"},
        {5, "User not found"},
        {6, "No message found"},
        {7, "Connection error"},
        {8, "Query error"},
        {9, "Fill all text fields"},
        {10, "Passwords are not equal"}
    };

public:
    Application();
    ~Application() = default;

public slots:
    void signIn();
    void signUp();

    void registration();
    void authorization();
};

#endif // APPLICATION_H
