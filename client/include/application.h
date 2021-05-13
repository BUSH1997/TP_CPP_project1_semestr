#ifndef APPLICATION_H
#define APPLICATION_H

#include "mainwidget.h"
#include "authorizwindow.h"
#include "registrationwindow.h"


#include <memory>
#include <QFrame>
#include <QObject>

namespace Ui {
class Frame;
}

namespace Ui {
class RegistrationWindow;
}

class Application: public QObject {
    Q_OBJECT
private:
    std::unique_ptr<MainWidget> mainWidget;
    std::unique_ptr<AuthorizWindow> authorizWindow;
    std::unique_ptr<RegistrationWindow> registrationWindow;

public:
    Application();
    ~Application() = default;

public slots:
    void signIn();


};

#endif // APPLICATION_H
