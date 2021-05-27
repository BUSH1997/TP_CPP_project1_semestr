#include "application.h"

#include "./ui_authorizwindow.h"
#include "./ui_registrationwindow.h"

#define CORRECT_DATA 0
#define CONNECT_ERROR 1
#define UNCORRECT_DATA 2
#define UNEQUAL_PASSWORDS 3
#define EMPTY_FIELDS 4
#define USER_EXIST 5
#define USER_DOESNT_EXIST 6
#define UNCORRECT_LOGIN_PASSWORD 7

Application::Application() {
    mainWidget = std::make_unique<MainWidget>();
    authorizWindow = std::make_unique<AuthorizWindow>();
    registrationWindow = std::make_unique<RegistrationWindow>();

    auto screenGeometry = QGuiApplication::screens().first()->availableGeometry();
    int x = (screenGeometry.width() - authorizWindow->width()) / 2;
    int y = (screenGeometry.height() - authorizWindow->height()) / 2;
    authorizWindow->move(x, y);

    x = (screenGeometry.width() - registrationWindow->width()) / 2;
    y = (screenGeometry.height() - registrationWindow->height()) / 2;
    registrationWindow->move(x, y);

    authorizWindow->show();

    connect(authorizWindow->ui->signInButton, SIGNAL(clicked()),
            this, SLOT(signIn()));

    connect(authorizWindow->ui->signUpButton, SIGNAL(clicked()),
            registrationWindow.get(), SLOT(show()));

    connect(authorizWindow->ui->signUpButton, SIGNAL(clicked()),
            authorizWindow.get(), SLOT(hide()));

//    --------------------------

    connect(registrationWindow->ui->signUpButton, SIGNAL(clicked()),
            this, SLOT(signUp()));

    connect(registrationWindow->ui->signInButton, SIGNAL(clicked()),
            authorizWindow.get(), SLOT(show()));

    connect(registrationWindow->ui->signInButton, SIGNAL(clicked()),
            registrationWindow.get(), SLOT(hide()));

//    client->connect(ep);

}


//void Application::authorization() {
//    QDir dir("cache/sessions");
//    QFile f(dir.path() + "/" + authorizWindow->ui->loginLine->text());
//    if (!f.open(QIODevice::ReadOnly)) {
//        return;
//    }

//    QByteArray array = f.read(65536);
//    std::string str(array.constData(), array.size());
//    JsonData sessionInformation = JsonParser::jsonToJsonData(str);

//    mainWidget->readSessionInformation(sessionInformation);
//    mainWidget->contactList->readSessionInformation(sessionInformation);
//}


void Application::authorization() {
    mainWidget->readCache();
}


void Application::signIn() {
    authorizWindow->ui->warningLabel->hide();

    auto reply = authorizWindow->sendData();
    if (!reply.errorDescription) {
        mainWidget->readSessionInformation(reply);
        mainWidget->contactList->readSessionInformation(reply);
        mainWidget->dialogMap->readSessionInformation(reply);

        std::string temp = reply.users.back().login + " # " + std::to_string(reply.users.back().userId);
        mainWidget->setWindowTitle(QString::fromStdString(temp));

        authorization();
        mainWidget->handleAuthorizeReply(reply);

        mainWidget->show();
        authorizWindow->close();
        registrationWindow->close();
        return;
    }

    authorizWindow->ui->warningLabel->show();
    authorizWindow->ui->warningLabel->setText(errorMap[reply.errorDescription]);
    return;

}


void Application::registration() {
//    QDir dir("cache");
//    QFile f(dir.path() + "/usersId");
//    if (!f.open(QIODevice::ReadWrite)) {
//        return;
//    }
//    QByteArray byteArray = f.read(65536);
//    std::string str(byteArray.constData(), byteArray.length());

//    auto l = str.rfind('{') + 1;
//    auto r = str.rfind('}');
//    if (l == std::string::npos)
//        return;
//    auto number = std::stoul(std::string(str, l, r)) + 1;

//    std::string numberStr = "{" + std::to_string(number) + "}" + "\n";
//    f.write(QByteArray(numberStr.c_str(), numberStr.length()));
//    f.close();

//    JsonData newUser;
//    UserData user;
//    user.name = registrationWindow->ui->nameLine->text().toStdString();
//    user.userId = number + 1;
//    user.name = registrationWindow->ui->nameLine->text().toStdString();
//    user.login = registrationWindow->ui->loginLine->text().toStdString();
//    user.password = registrationWindow->ui->passwordLine->text().toStdString();
//    newUser.users.push_back(user);

//    QDir dir2("cache/sessions");
//    QFile f2(dir2.path() + "/" + QString::fromStdString(newUser.users.back().login));
//    if (!f2.open(QIODevice::WriteOnly)) {
//        return;
//    }

//    auto t = JsonParser::jsonDataToJson(newUser);
//    QByteArray byteArray2(t.c_str(), t.length());

//    f2.write(byteArray2);
//    f2.close();

//    mainWidget->readSessionInformation(newUser);
}

void Application::signUp() {
    registrationWindow->ui->warningLabel->hide();

    authorizWindow->ui->warningLabel->hide();

    auto reply = registrationWindow->sendData();
    if (!reply.errorDescription) {
//        authorization();

        mainWidget->readSessionInformation(reply);
        mainWidget->contactList->readSessionInformation(reply);
        mainWidget->dialogMap->readSessionInformation(reply);

        std::string temp = reply.users.back().login + " # " + std::to_string(reply.users.back().userId);
        mainWidget->setWindowTitle(QString::fromStdString(temp));

        mainWidget->show();
        authorizWindow->close();
        registrationWindow->close();
        return;
    }

    registrationWindow->ui->warningLabel->show();
    registrationWindow->ui->warningLabel->setText(errorMap[reply.errorDescription]);
    return;
}
