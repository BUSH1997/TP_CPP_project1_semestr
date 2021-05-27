#ifndef NEWDIALOGWIDGET_H
#define NEWDIALOGWIDGET_H

//#include "serverConnection.h"
#include "sync_client.h"
#include "parser.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGuiApplication>
#include <QScreen>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QKeyEvent>
#include <QDir>

#include <chrono>
#include <memory>
#include <iostream>

extern std::unique_ptr<talk_to_svr> client;
extern ip::tcp::endpoint ep;

class NewDialogWidget : public QWidget {
    Q_OBJECT

public:
    NewDialogWidget(QWidget *parent = nullptr);
    ~NewDialogWidget();

    std::unique_ptr<QLineEdit> signal;

    void readSessionInformation(JsonData sessionInformation);

protected:
    void closeEvent(QCloseEvent*);
    void keyPressEvent(QKeyEvent* e);

public slots:
    void updateData();
    void sendData();

private:
    JsonData sessionInformation;

    QString userIdStr;

    std::unique_ptr<QVBoxLayout> newDialogLayout;

    std::unique_ptr<QWidget> newDialogTopWidget;
    std::unique_ptr<QHBoxLayout> newDialogTopLayout;

    std::unique_ptr<QLabel> userIdLabel;
    std::unique_ptr<QLineEdit> userIdLine;

    std::unique_ptr<QWidget> newDialogBottomWidget;
    std::unique_ptr<QHBoxLayout> newDialogBottomLayout;

    std::unique_ptr<QLabel> warningLabel;
    std::unique_ptr<QWidget> spacer;
    std::unique_ptr<QPushButton> addButton;

    std::map<int, QString> errorMap{
        {0, "Success"},
        {1, "Invalid Password"},
        {2, "Invalid Login"},
        {3, "Login already exist"},
        {4, "User was deleted"},
        {5, "User not found"},
        {6, "No message found"},
        {7, "Connection error"},
        {8, "Query error"},
        {9, "Fill all text fields"},
        {10, "Passwords are not equal"}
    };

};

#endif // NEWDIALOGWIDGET_H
