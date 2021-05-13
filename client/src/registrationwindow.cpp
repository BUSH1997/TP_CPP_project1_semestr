#include "registrationwindow.h"
#include "ui_registrationwindow.h"

#include <iostream>

#define CORRECT_DATA 0
#define CONNECT_ERROR 1
#define UNCORRECT_DATA 2
#define UNEQUAL_PASSWORDS 3

RegistrationWindow::RegistrationWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->nameLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData())
                     );

    QObject::connect(ui->loginLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData())
                     );

    QObject::connect(ui->passwordLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData())
                     );

    QObject::connect(ui->repeatPasswordLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData())
                     );

    QObject::connect(ui->signUpButton, SIGNAL(clicked()),
                     this, SLOT(sendData())
                     );

    ui->passwordLine->setEchoMode(QLineEdit::Password);
    ui->repeatPasswordLine->setEchoMode(QLineEdit::Password);

    ui->warningLabel->hide();

}

void RegistrationWindow::updateData() {
    data.name = ui->nameLine->text().toUtf8().constData();
    data.login = ui->loginLine->text().toUtf8().constData();
    data.password = ui->passwordLine->text().toUtf8().constData();
    repeatPasswordStr = ui->repeatPasswordLine->text().toUtf8().constData();

    data.status = false;

    std::cout << "name: " << data.name << std::endl;
    std::cout << "login: " << data.login << std::endl;
    std::cout << "password: " << data.password << std::endl;
    std::cout << "repeat password: " << repeatPasswordStr << std::endl;

}

int RegistrationWindow::sendData() {
    updateData();

    if ((data.password == "") || (repeatPasswordStr == "")) {
        ui->warningLabel->setText("Fill the password fields");
        return UNEQUAL_PASSWORDS;
    }

    if (data.password != repeatPasswordStr) {
        std::cout << "passwords are not equal" << std::endl;
        return UNEQUAL_PASSWORDS;
    }

    auto dataStr = JsonParser::jsonDataToJson(data);
    std::string replyStr = "";
    if (serverConnection.start()) {
        serverConnection.stop();
        return CONNECT_ERROR;
    }
    replyStr = serverConnection.echoWriteRead(dataStr);
    serverConnection.stop();

    if (replyStr != dataStr) {
        std::cout << "message not delivered" << std::endl;
        return UNCORRECT_DATA;
    } else {
        data.status = true;
        std::cout << "message was sended" << std::endl;
        return CORRECT_DATA;
    }
}



RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}
