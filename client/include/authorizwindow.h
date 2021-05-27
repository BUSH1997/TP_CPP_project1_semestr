#ifndef FRAME_H
#define FRAME_H

#include <QFrame>
#include <QKeyEvent>
#include "serverConnection.h"
#include "parser.h"
#include "sync_client.h"

extern std::unique_ptr<talk_to_svr> client;
extern ip::tcp::endpoint ep;

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
    JsonData sendData();

public:
    JsonData data;

};

#endif // FRAME_H
