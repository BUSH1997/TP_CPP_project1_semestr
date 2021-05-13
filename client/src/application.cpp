#include "application.h"

#include "./ui_authorizwindow.h"
#include "./ui_registrationwindow.h"

#define CONNECT_ERROR 1
#define UNCORRECT_DATA 2
#define CORRECT_DATA 0

Application::Application() {
    mainWidget = std::make_unique<MainWidget>();
    authorizWindow = std::make_unique<AuthorizWindow>();
    registrationWindow = std::make_unique<RegistrationWindow>();

    authorizWindow->show();

    connect(authorizWindow->ui->signInButton, SIGNAL(clicked()),
                     this, SLOT(signIn()));


//    ------------------------

    QObject::connect(authorizWindow->ui->signUpButton, SIGNAL(clicked()),
                     registrationWindow.get(), SLOT(show()));

    QObject::connect(authorizWindow->ui->signUpButton, SIGNAL(clicked()),
                     authorizWindow.get(), SLOT(hide()));

//    --------------------------

    QObject::connect(registrationWindow->ui->signUpButton, SIGNAL(clicked()),
                     mainWidget.get(), SLOT(show()));

    QObject::connect(registrationWindow->ui->signUpButton, SIGNAL(clicked()),
                     authorizWindow.get(), SLOT(close()));

    QObject::connect(registrationWindow->ui->signUpButton, SIGNAL(clicked()),
                     registrationWindow.get(), SLOT(close()));

//    --------------------------------

    QObject::connect(registrationWindow->ui->signInButton, SIGNAL(clicked()),
                     authorizWindow.get(), SLOT(show()));

    QObject::connect(registrationWindow->ui->signInButton, SIGNAL(clicked()),
                     registrationWindow.get(), SLOT(hide()));

//    ----------------------------


}

void Application::signIn() {
    authorizWindow->ui->warningLabel->hide();
    if (authorizWindow->sendData()) {
        authorizWindow->ui->warningLabel->show();
        authorizWindow->ui->warningLabel->setText("Uncorrect login or password");
        return;
    }

    switch (authorizWindow->sendData()) {
        case CONNECT_ERROR: {
            authorizWindow->ui->warningLabel->show();
            authorizWindow->ui->warningLabel->setText("No connection");
            return;
        }

        case UNCORRECT_DATA: {
            authorizWindow->ui->warningLabel->show();
            authorizWindow->ui->warningLabel->setText("Uncorrect login or password");
            return;
        }

        case CORRECT_DATA: {
            break;
        }

        default:
            return;
    }

    mainWidget->show();
    authorizWindow->close();
    registrationWindow->close();
}
