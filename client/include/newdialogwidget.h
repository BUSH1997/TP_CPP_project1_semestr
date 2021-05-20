#ifndef NEWDIALOGWIDGET_H
#define NEWDIALOGWIDGET_H

#include "serverConnection.h"
#include "Parser.h"

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
    ServerConnection serverConnection;

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

};

#endif // NEWDIALOGWIDGET_H
