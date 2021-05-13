#include "application.h"

#include "./ui_authorizwindow.h"
#include "./ui_registrationwindow.h"

#define CORRECT_DATA 0
#define CONNECT_ERROR 1
#define UNCORRECT_DATA 2
#define UNEQUAL_PASSWORDS 3

Application::Application() {
    mainWidget = std::make_unique<MainWidget>();
    authorizWindow = std::make_unique<AuthorizWindow>();
    registrationWindow = std::make_unique<RegistrationWindow>();

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

//    ----------------------------


}

void Application::signIn() {
    authorizWindow->ui->warningLabel->hide();

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

void Application::signUp() {
    registrationWindow->ui->warningLabel->hide();

    switch (registrationWindow->sendData()) {
        case UNEQUAL_PASSWORDS: {
            registrationWindow->ui->warningLabel->show();
            registrationWindow->ui->warningLabel->setText("Passwords are not equal");
            return;
        }

        case CONNECT_ERROR: {
            registrationWindow->ui->warningLabel->show();
            registrationWindow->ui->warningLabel->setText("No connection");
            return;
        }

        case UNCORRECT_DATA: {
            registrationWindow->ui->warningLabel->show();
            registrationWindow->ui->warningLabel->setText("Uncorrect login or password");
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
