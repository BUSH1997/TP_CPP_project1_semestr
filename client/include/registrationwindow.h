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
    void sendData();

private:
    JsonData data;
    std::string repeatPasswordStr;
};

#endif // REGISTRATIONWINDOW_H
