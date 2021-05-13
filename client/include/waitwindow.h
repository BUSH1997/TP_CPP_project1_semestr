#pragma once

#ifndef WAITWINDOW_H
#define WAITWINDOW_H

#include <QWidget>

class WaitWindow: public QWidget {
    Q_OBJECT

public:
    WaitWindow(QWidget* parent = nullptr);
    ~WaitWindow();
};

#endif // WAITWINDOW_H
