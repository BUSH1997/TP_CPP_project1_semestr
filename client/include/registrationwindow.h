#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QWidget>
#include "Parser.h"
#include "serverConnection.h"

namespace Ui {
class RegistrationWindow;
}

class RegistrationWindow : public QWidget
{
    Q_OBJECT

public:
    Ui::RegistrationWindow *ui;

    explicit RegistrationWindow(QWidget *parent = nullptr);
    ~RegistrationWindow();

public slots:
    void updateData();
    int sendData();

private:
    JsonData data;
    std::string repeatPasswordStr;
    ServerConnection serverConnection;
};

#endif // REGISTRATIONWINDOW_H
