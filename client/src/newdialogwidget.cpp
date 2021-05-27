#include "newdialogwidget.h"

NewDialogWidget::NewDialogWidget(QWidget *parent) {
    newDialogLayout = std::make_unique<QVBoxLayout>(this);
    setLayout(newDialogLayout.get());

    signal = std::make_unique<QLineEdit>();

    QDir cacheDir;
    cacheDir.mkdir("cache");

    setWindowTitle("New Dialog");
    resize(300, 100);

    auto screenGeometry = QGuiApplication::screens().first()->availableGeometry();
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    newDialogTopWidget = std::make_unique<QWidget>();
    newDialogTopLayout = std::make_unique<QHBoxLayout>(newDialogTopWidget.get());
    newDialogLayout->addWidget(newDialogTopWidget.get());
    newDialogTopWidget->setLayout(newDialogTopLayout.get());

    userIdLabel = std::make_unique<QLabel>("User login: ", newDialogTopWidget.get());
    newDialogTopLayout->addWidget(userIdLabel.get(), 2);

    userIdLine = std::make_unique<QLineEdit>(newDialogTopWidget.get());
    newDialogTopLayout->addWidget(userIdLine.get(), 7);

    newDialogBottomWidget = std::make_unique<QWidget>(this);
    newDialogBottomLayout = std::make_unique<QHBoxLayout>(newDialogBottomWidget.get());
    newDialogBottomWidget->setLayout(newDialogBottomLayout.get());
    newDialogLayout->addWidget(newDialogBottomWidget.get());

    warningLabel = std::make_unique<QLabel>("Not find", newDialogBottomWidget.get());
    newDialogBottomLayout->addWidget(warningLabel.get(), 5);

    spacer = std::make_unique<QWidget>(newDialogBottomWidget.get());
    newDialogBottomLayout->addWidget(spacer.get(), 5);
    spacer->show();

    addButton = std::make_unique<QPushButton>(newDialogBottomWidget.get());
    newDialogBottomLayout->addWidget(addButton.get(), 2);

    warningLabel->hide();
    addButton->setText("Add");

    connect(userIdLine.get(),  SIGNAL(editingFinished()),
            this, SLOT(updateData()));

    connect(addButton.get(), SIGNAL(clicked()),
            this, SLOT(sendData()));
}


void NewDialogWidget::updateData() {
    userIdStr = userIdLine->text();
}


void NewDialogWidget::sendData() {
    updateData();
    warningLabel->show();
    spacer->hide();

    if (userIdStr.isEmpty()) {
        warningLabel->setText("Fill the text field");
        return;
    }

    auto login = userIdStr.toStdString();
    JsonData data;
    data.users.push_back(UserData());
    data.users.back().login = login;
    data.requestStatus = 1;
    data.errorDescription = 0;
    data.requestType = "new_dialog";

    auto reply = client->get_user(data);


//    auto replyError = reply.errorDescription;

//    if (replyError) {
//        std::cout << "error" << std::endl;
//        warningLabel->setText("error");
//        return;
//    }

//    signal->setText(QString::fromStdString(JsonParser::jsonDataToJson(reply)));
//    warningLabel->setText("");
//    userIdLine->clear();
//    this->close();

    if (!reply.errorDescription) {
        signal->setText(QString::fromStdString(JsonParser::jsonDataToJson(reply)));
        warningLabel->setText("");
        userIdLine->clear();
        this->close();
        return;
    }

    warningLabel->setText(errorMap[reply.errorDescription]);




//    ----------------------------

//    auto userId = userIdStr.toULong();
//    JsonData data;
//    data.users.push_back(UserData());
//    data.messages.push_back(MessageData());

//    data.messages.back().transmitterId = sessionInformation.users.back().userId;
//    data.messages.back().receiverId = userId;
//    data.messages.back().date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

//    if (!data.messages.back().receiverId) {
//        warningLabel->setText("Uncorrect UserId");
//        return;
//    }

//    auto dataStr = JsonParser::jsonDataToJson(data);
//    std::string replyStr = "";
//    if (serverConnection.start()) {
//        serverConnection.stop();
//        warningLabel->setText("No connection");
//        return;
//    }
//    replyStr = serverConnection.echoWriteRead(dataStr);
//    serverConnection.stop();

//    if (replyStr != dataStr) {
//        warningLabel->setText("Message not delivered");
//        return;
//    } else {
//        data.requestStatus = 1;
//        std::cout << "message was sended" << std::endl;

////        -------------------------
//        auto userId = userIdStr.toULong();

//        JsonData receive;
//        UserData receiveUser;
//        receiveUser.userId = userId;
//        receive.users.push_back(receiveUser);


//        MessageData message;
//        message.transmitterId = userId;
//        message.receiverId = sessionInformation.users.back().userId;
//        message.date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
//        receive.users.back().name = "contact #" + std::to_string(message.transmitterId);
//        receive.messages.push_back(message);

//        auto receiveStr = JsonParser::jsonDataToJson(receive);
//        std::string replyStr = "";
//        if (serverConnection.start()) {
//            serverConnection.stop();
//            warningLabel->setText("No connection");
//            return;
//        }
//        replyStr = serverConnection.echoWriteRead(receiveStr);
//        serverConnection.stop();

////        -------------------------

//        signal->setText(QString::fromStdString(replyStr));
//        warningLabel->setText("");
//        userIdLine->clear();
//        this->close();
//    }
}


void NewDialogWidget::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Enter) {
        addButton->clicked(true);
    }
}


void NewDialogWidget::closeEvent(QCloseEvent *) {
    this->close();
}


void NewDialogWidget::readSessionInformation(JsonData sessionInformation) {
    this->sessionInformation = sessionInformation;
}


NewDialogWidget::~NewDialogWidget() {
}
