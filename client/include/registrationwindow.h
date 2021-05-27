#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QWidget>
#include <QKeyEvent>
#include "parser.h"
//#include "serverConnection.h"
#include "sync_client.h"

extern std::unique_ptr<talk_to_svr> client;
extern ip::tcp::endpoint ep;

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

protected:
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *);

public slots:
    void updateData();
    JsonData sendData();

public:
    JsonData data;

private:

    std::string repeatPasswordStr;
//    ServerConnection serverConnection;
};

#endif // REGISTRATIONWINDOW_H
