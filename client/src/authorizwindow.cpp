#include "authorizwindow.h"
#include "./ui_authorizwindow.h"

#include <iostream>
#include <QFileInfo>
#include <QFile>
#include <boost/thread.hpp>

#define CORRECT_DATA 0
#define CONNECT_ERROR 1
#define UNCORRECT_DATA 2
#define UNEQUAL_PASSWORDS 3

#define USER_EXIST 5
#define USER_DOESNT_EXIST 6
#define UNCORRECT_LOGIN_PASSWORD 7


#define EMPTY_FIELDS 9

AuthorizWindow::AuthorizWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame)
{
    data.users.push_back(UserData());
    ui->setupUi(this);

    QObject::connect(ui->loginLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData()));

    QObject::connect(ui->passwordLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData()));

//    QObject::connect(ui->signInButton, SIGNAL(clicked()),
//                     this, SLOT(sendData()));

    ui->passwordLine->setEchoMode(QLineEdit::Password);
    ui->warningLabel->hide();


}


void AuthorizWindow::updateData() {
    data.users.back().login = ui->loginLine->text().toUtf8().constData();
    data.users.back().password = ui->passwordLine->text().toUtf8().constData();

    data.requestStatus = 0;

//    std::cout << "login: " << data.users.back().login << std::endl;
//    std::cout << "password: " << data.users.back().password << std::endl;
}


JsonData AuthorizWindow::sendData() {
    updateData();
//    data.users.back().updateDate = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

    auto temp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&temp));
    data.users.back().updateDate = timestr;

    data.requestType = "authorization";
    data.requestStatus = 1;
    data.errorDescription = 0;


    if (data.users.back().login.empty() || data.users.back().password.empty()) {
        std::cout << "fill all text fields" << std::endl;
        JsonData temp;
        temp.errorDescription = EMPTY_FIELDS;
        return temp;
    }

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
        reply = client->authorization(data);
        std::cout << "reply" << std::endl;
        std::cout << JsonParser::jsonDataToJson(reply) << std::endl;

//        -----------------------


//        reply = client->authorization(data);
//        std::thread t(&talk_to_svr::loop, &*client);
//        t.detach();


    }
    catch(boost::system::system_error & err) {
        std::cout << "client terminated " << client->username()
                  << ": " << err.what() << std::endl;
    }

//    data = reply;

    if (!reply.errorDescription) {
        std::cout << "data was sended" << std::endl;
    }

//    return reply.errorDescription;

    return reply;

//    =======================

//    //    =======================
//    std::size_t reply;
//    try {
//        client->connect(ep);
//        reply = client->authorization(data);
//        std::thread t(&talk_to_svr::loop, &*client);
//        t.detach();
//    }
//    catch(boost::system::system_error & err) {
//        std::cout << "client terminated " << client->username()
//                  << ": " << err.what() << std::endl;
//    }

//    if (!reply) {
//        std::cout << "data was sended" << std::endl;
//    }

//    return reply;

////    =======================

//    int result;

//    auto dataStr = JsonParser::jsonDataToJson(data);
//    std::string replyStr = "";
//    if (serverConnection.start()) {
//        result = CONNECT_ERROR;
//        serverConnection.stop();
//        return result;
//    }
//    replyStr = serverConnection.echoWriteRead(dataStr);
//    serverConnection.stop();

//    if (replyStr != dataStr) {
//        std::cout << "message not delivered" << std::endl;
//        return UNCORRECT_DATA;
//    }

//    QFileInfo fileInfo("cache/sessions/" + QString::fromStdString(data.users.back().login));
//    if (!fileInfo.exists() || !fileInfo.isFile()) {
//        std::cout << "user are not exist" << std::endl;
//        return USER_DOESNT_EXIST;
//    }

//    QFile file("cache/sessions/" + QString::fromStdString(data.users.back().login));
//    if (!file.open(QIODevice::ReadOnly)) {
//        return UNCORRECT_DATA;
//    }

//    QByteArray byteArray = file.read(65536);
//    std::string str(byteArray.constData(), byteArray.length());
//    file.close();
//    if (data.users.back().password != JsonParser::jsonToJsonData(str).users.back().password) {
//        return UNCORRECT_LOGIN_PASSWORD;
//    }

//    std::cout << "data was sended" << std::endl;

//    data.requestStatus = 1;
//    return CORRECT_DATA;
}


void AuthorizWindow::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Enter) {
        ui->signInButton->clicked(true);
        return;
    }
}


void AuthorizWindow::closeEvent(QCloseEvent *) {
    this->close();
}


AuthorizWindow::~AuthorizWindow()
{
    delete ui;
}
