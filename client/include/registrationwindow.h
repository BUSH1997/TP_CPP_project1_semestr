#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QWidget>

class RegistrationWindow: public QWidget {
    Q_OBJECT

public:
    RegistrationWindow(QWidget* parent = nullptr);
    ~RegistrationWindow();
};

#endif // REGISTRATIONWINDOW_H
