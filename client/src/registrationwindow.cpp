#include "registrationwindow.h"
#include "ui_registrationwindow.h"

#include <QFileInfo>

#include <iostream>

#define CORRECT_DATA 0
#define CONNECT_ERROR 1
#define UNCORRECT_DATA 2
//#define UNEQUAL_PASSWORDS 3
//#define EMPTY_FIELDS 4
#define USER_EXIST 5

#define EMPTY_FIELDS 9
#define UNEQUAL_PASSWORDS 10

RegistrationWindow::RegistrationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);

    data.users.push_back(UserData());

    QObject::connect(ui->nameLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData()));

    QObject::connect(ui->loginLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData()));

    QObject::connect(ui->passwordLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData()));

    QObject::connect(ui->repeatPasswordLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData()));

//    QObject::connect(ui->signUpButton, SIGNAL(clicked()),
//                     this, SLOT(sendData()));

    ui->passwordLine->setEchoMode(QLineEdit::Password);
    ui->repeatPasswordLine->setEchoMode(QLineEdit::Password);

    ui->warningLabel->hide();
}


void RegistrationWindow::updateData() {
    data.users.back().name = ui->nameLine->text().toUtf8().constData();
    data.users.back().login = ui->loginLine->text().toUtf8().constData();
    data.users.back().password = ui->passwordLine->text().toUtf8().constData();
    repeatPasswordStr = ui->repeatPasswordLine->text().toUtf8().constData();

    data.requestStatus = 0;

//    std::cout << "name: " << data.users.back().name << std::endl;
//    std::cout << "login: " << data.users.back().login << std::endl;
//    std::cout << "password: " << data.users.back().password << std::endl;
//    std::cout << "repeat password: " << repeatPasswordStr << std::endl;
}


JsonData RegistrationWindow::sendData() {
    updateData();

    if (data.users.back().name.empty() || data.users.back().login.empty() ||
            data.users.back().password.empty() || repeatPasswordStr.empty()) {
        std::cout << "fill all text fields" << std::endl;
        JsonData temp;
        temp.errorDescription = EMPTY_FIELDS;
        return temp;
    }

    if (data.users.back().password != repeatPasswordStr) {
        std::cout << "passwords are not equal" << std::endl;
        JsonData temp;
        temp.errorDescription = UNEQUAL_PASSWORDS;
        return temp;
    }

//    data.users.back().updateDate = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

    auto temp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&temp));
    data.users.back().updateDate = timestr;


    data.requestType = "registration";
    data.requestStatus = 1;
    data.errorDescription = 0;

//    std::cout << JsonParser::jsonDataToJson(data) << std::endl;

    //    =======================
    JsonData reply;
    try {
        if (!client->isOpen()) {
            client->connect(ep);
            std::thread t(&talk_to_svr::loop, &*client);
            t.detach();
        }

        std::this_thread::sleep_for(std::chrono::microseconds(100));
        std::cout << JsonParser::jsonDataToJson(data) << std::endl;
        reply = client->registration(data);
        std::cout << "reply" << std::endl;
        std::cout << JsonParser::jsonDataToJson(reply) << std::endl;

    }
    catch(boost::system::system_error & err) {
        std::cout << "client terminated " << client->username()
                  << ": " << err.what() << std::endl;
    }

//        data = reply;

    if (!reply.errorDescription) {
        std::cout << "data was delivered" << std::endl;
    }



//    return reply.errorDescription;

    return reply;

//    =======================

//    //    =======================
//    std::size_t reply;
//    try {
//        client->connect(ep);
//        reply = client->registration(data);
//        std::thread t(&talk_to_svr::loop, &*client);
//        t.detach();
//    }
//    catch(boost::system::system_error & err) {
//        std::cout << "client terminated " << client->username()
//                  << ": " << err.what() << std::endl;
//    }

//    if (!reply) {
//        std::cout << "data was delivered" << std::endl;
//    }

//    return reply;

////    =======================

//    auto dataStr = JsonParser::jsonDataToJson(data);
//    std::string replyStr = "";
//    if (serverConnection.start()) {
//        serverConnection.stop();
//        return CONNECT_ERROR;
//    }
//    replyStr = serverConnection.echoWriteRead(dataStr);
//    serverConnection.stop();

//    if (replyStr != dataStr) {
//        std::cout << "message not delivered" << std::endl;
//        return UNCORRECT_DATA;
//    }

//    QFileInfo fileInfo("cache/sessions/" + QString::fromStdString(data.users.back().login));
//    if (fileInfo.exists() && fileInfo.isFile()) {
//        std::cout << "user already exist" << std::endl;
//        return USER_EXIST;
//    }

//    data.requestStatus = 1;
//    std::cout << "message was sended" << std::endl;
//    return CORRECT_DATA;
}


void RegistrationWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Enter) {
        ui->signUpButton->clicked(true);
        return;
    }
}


void RegistrationWindow::closeEvent(QCloseEvent *) {
    this->close();
}


RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}
