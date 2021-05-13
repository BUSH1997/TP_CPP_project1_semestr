#include "registrationwindow.h"
#include "ui_registrationwindow.h"

#include <iostream>

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

    ui->passwordWarningLabel->hide();

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

void RegistrationWindow::sendData() {
    if (data.password != repeatPasswordStr) {
        ui->passwordWarningLabel->show();

        std::cout << "passwords are not equal" << std::endl;
        return;
    }

    data.status = true;
    ui->passwordWarningLabel->hide();

    std::cout << "data was sended" << std::endl;
}



RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}
