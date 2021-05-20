#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include <QKeyEvent>
#include "serverConnection.h"
#include "Parser.h"

namespace Ui {
class Frame;
}

class AuthorizWindow : public QFrame
{
    Q_OBJECT

public:
    Ui::Frame *ui;

    explicit AuthorizWindow(QWidget *parent = nullptr);
    ~AuthorizWindow();

protected:
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent *);

public slots:
    void updateData();
    int sendData();

private:
    JsonData data;
    ServerConnection serverConnection;

};

#endif // FRAME_H
