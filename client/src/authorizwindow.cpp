#include "authorizwindow.h"
#include "./ui_authorizwindow.h"

#include <iostream>

#define CONNECT_ERROR 1
#define UNCORRECT_DATA 2
#define CORRECT_DATA 0

AuthorizWindow::AuthorizWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);

    QObject::connect(ui->loginLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData())
                     );


    QObject::connect(ui->passwordLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData())
                     );

    QObject::connect(ui->signInButton, SIGNAL(clicked()),
                     this, SLOT(sendData())
                     );

    ui->passwordLine->setEchoMode(QLineEdit::Password);
    ui->warningLabel->hide();

}

void AuthorizWindow::updateData() {
    data.login = ui->loginLine->text().toUtf8().constData();
    data.password = ui->passwordLine->text().toUtf8().constData();

    data.status = false;

    std::cout << "login: " << data.login << std::endl;
    std::cout << "password: " << data.password << std::endl;


}

int AuthorizWindow::sendData() {
    int result;
    auto dataStr = JsonParser::jsonDataToJson(data);
    std::string replyStr = "";
    if (serverConnection.start()) {
        result = CONNECT_ERROR;
        serverConnection.stop();
        return result;
    }
    replyStr = serverConnection.echoWriteRead(dataStr);
    serverConnection.stop();

    if (replyStr != dataStr) {
        std::cout << "message not delivered" << std::endl;
        result = UNCORRECT_DATA;
    } else {
        data.status = true;
        result = CORRECT_DATA;
        std::cout << "message was sended" << std::endl;
    }

    std::cout << "data was sended" << std::endl;

    return result;
}

AuthorizWindow::~AuthorizWindow()
{
    delete ui;
}
