#include "authorizwindow.h"
#include "./ui_authorizwindow.h"

#include <iostream>
#include <QFileInfo>
#include <QFile>

#define CORRECT_DATA 0
#define CONNECT_ERROR 1
#define UNCORRECT_DATA 2
#define UNEQUAL_PASSWORDS 3
#define EMPTY_FIELDS 4
#define USER_EXIST 5
#define USER_DOESNT_EXIST 6
#define UNCORRECT_LOGIN_PASSWORD 7

AuthorizWindow::AuthorizWindow(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Frame)
{
    ui->setupUi(this);

    QObject::connect(ui->loginLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData()));

    QObject::connect(ui->passwordLine, SIGNAL(editingFinished()),
                     this, SLOT(updateData()));

    QObject::connect(ui->signInButton, SIGNAL(clicked()),
                     this, SLOT(sendData()));

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
    updateData();
    int result;

    if (data.login.empty() || data.password.empty()) {
        std::cout << "fill all text fields" << std::endl;
        return EMPTY_FIELDS;
    }
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
        return UNCORRECT_DATA;
    }

    QFileInfo fileInfo("cache/sessions/" + QString::fromStdString(data.login));
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        std::cout << "user are not exist" << std::endl;
        return USER_DOESNT_EXIST;
    }

    QFile file("cache/sessions/" + QString::fromStdString(data.login));
    if (!file.open(QIODevice::ReadOnly)) {
        return UNCORRECT_DATA;
    }

    QByteArray byteArray = file.read(65536);
    std::string str(byteArray.constData(), byteArray.length());
    file.close();
    if (data.password != JsonParser::jsonToJsonData(str).password) {
        return UNCORRECT_LOGIN_PASSWORD;
    }

    std::cout << "data was sended" << std::endl;

    data.status = true;
    return CORRECT_DATA;
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
