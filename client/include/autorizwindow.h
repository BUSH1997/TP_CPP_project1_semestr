#pragma once

#ifndef AUTORIZWINDOW_H
#define AUTORIZWINDOW_H

#include <QWidget>

class AutorizWindow: public QWidget {
    Q_OBJECT

public:
    AutorizWindow(QWidget* parent = nullptr);
    ~AutorizWindow();
};


#endif // AUTORIZWINDOW_H
