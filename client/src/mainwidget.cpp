#include "mainwidget.h"

//extern std::unique_ptr<QLineEdit> inputMessage;

//static std::string parser(std::string data) {
//    std::list<char> list;

//    if (data == "ping") {
//        return data;
//    }

//    std::string result = data;

//    int i = 0;
//    for (auto it = data.begin(); it != data.end(); ++it, ++i) {

//        if (*it == '{') {
//            list.push_back('1');
//        }
//        if (*it == '}') {
//            list.pop_back();
//            if (list.empty()) {
//                result = std::string(data, 0, i + 1);
//                break;
//            }
//        }
//    }
//    return result;
//}



MainWidget::MainWidget(QWidget* parent): QWidget(parent) {
    QDir audioDir;
    audioDir.mkdir("audio");

    QDir cacheDir;
    cacheDir.mkdir("cache");

//    client->getReceiveMessAddress(&MainWidget::receiveMessage, this);

    inputMessage = std::make_unique<QLineEdit>();
    inputMessage->hide();
    client->getInputMessageAddress(inputMessage.get());
    connect(inputMessage.get(), SIGNAL(textChanged(QString)),
            this, SLOT(receiveMessage(QString)));

    newDialogWidget = std::make_unique<NewDialogWidget>();
    connect(newDialogWidget.get()->signal.get(), SIGNAL(textChanged(QString)),
            this, SLOT(createNewDialog(QString)));

    dialogMessagePtr = std::make_unique<DialogMessagePtr>();

    auto screenGeometry = QGuiApplication::screens().first()->availableGeometry();
    resize(screenGeometry.width(), screenGeometry.height());
    setWindowTitle("YAMessanger");

    mainWindowLayoutWidget = std::make_unique<QWidget>(this);

    mainWindowLayout = std::make_unique<QHBoxLayout>(mainWindowLayoutWidget.get());
    setLayout(mainWindowLayout.get());

    rightBoardWidget = std::make_unique<QWidget>(mainWindowLayoutWidget.get());
    mainWindowLayout->addWidget(rightBoardWidget.get(), 2);

    leftBoardWidget = std::make_unique<QWidget>(mainWindowLayoutWidget.get());
    mainWindowLayout->addWidget(leftBoardWidget.get(), 7);

    rightBoardLayout = std::make_unique<QVBoxLayout>(rightBoardWidget.get());
    rightBoardWidget->setLayout(rightBoardLayout.get());

    contactsLabel = std::make_unique<QLabel>(rightBoardWidget.get());
    rightBoardLayout->addWidget(contactsLabel.get(), 1);
    contactsLabel->setText("Contacts: ");

//    ------------------------------
    testFooButton = std::make_unique<QPushButton>(rightBoardWidget.get());
    rightBoardLayout->addWidget(testFooButton.get(), 2);
    testFooButton.get()->setText("testFoo");
    connect(testFooButton.get(), SIGNAL(clicked()), this, SLOT(testSlot()));

    contactListWidget = std::make_unique<QListWidget>();
    rightBoardLayout->addWidget(contactListWidget.get(), 20);

    contactList = std::make_unique<ContactListClass>(sessionInformation);
    connect(contactListWidget.get(), SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(makeDialog(QListWidgetItem*)));

    addContactButton = std::make_unique<QPushButton>(rightBoardWidget.get());
    rightBoardLayout->addWidget(addContactButton.get(), 2);
    addContactButton->setText("Add contact");
    connect(addContactButton.get(), SIGNAL(clicked()), this, SLOT(addContact()));

//    ------------------------------
    leftBoardLayout = std::make_unique<QVBoxLayout>();
    leftBoardWidget->setLayout(leftBoardLayout.get());

    dialogWidget = std::make_unique<QListWidget>();
    leftBoardLayout->addWidget(dialogWidget.get(), 7);
    dialogWidget->setSelectionMode(QAbstractItemView::NoSelection);

    dialogMap = std::make_unique<DialogMapClass>(sessionInformation);


//====================================

    bottomBoardWidget = std::make_unique<QWidget>();
    leftBoardLayout->addWidget(bottomBoardWidget.get(), 2);
    bottomBoardWidget->hide();

    bottomBoardLayout = std::make_unique<QHBoxLayout>(bottomBoardWidget.get());
    bottomBoardWidget->setLayout(bottomBoardLayout.get());

    messageTextBrowser = std::make_unique<TextEdit>(bottomBoardWidget.get(), this);
    bottomBoardLayout->addWidget(messageTextBrowser.get(), 10);
    messageTextBrowser->setReadOnly(false);

    buttonsWidget = std::make_unique<QWidget>(bottomBoardWidget.get());
    bottomBoardLayout->addWidget(buttonsWidget.get(), 1);

    buttonsLayout = std::make_unique<QVBoxLayout>(buttonsWidget.get());
    buttonsWidget->setLayout(buttonsLayout.get());

    recordButton = std::make_unique<QPushButton>(buttonsWidget.get());
    buttonsLayout->addWidget(recordButton.get());
    recordButton->setText("&Record");
    connect(recordButton.get(), SIGNAL(clicked()), this, SLOT(recordAudio()));

    sendButton = std::make_unique<QPushButton>(buttonsWidget.get());
    buttonsLayout->addWidget(sendButton.get());
    sendButton->setText("&Send");
    connect(sendButton.get(), SIGNAL(clicked()), this, SLOT(sendMessage()));

    stopButton = std::make_unique<QPushButton>(buttonsWidget.get());
    buttonsLayout->addWidget(stopButton.get());
    stopButton->setText("&Stop");
    stopButton->hide();
    connect(stopButton.get(), SIGNAL(clicked()), this, SLOT(stopRecordAudio()));

    connect(recordButton.get(), SIGNAL(clicked()), recordButton.get(), SLOT(hide()));
    connect(recordButton.get(), SIGNAL(clicked()), stopButton.get(), SLOT(show()));

    connect(stopButton.get(), SIGNAL(clicked()), stopButton.get(), SLOT(hide()));
    connect(stopButton.get(), SIGNAL(clicked()), recordButton.get(), SLOT(show()));

//     ===============================

}


std::size_t& MainWidget::dialogId(JsonData& data) {
    if (data.messages.back().transmitterId == sessionInformation.users.back().userId) {
        return data.messages.back().receiverId;
    }
    return data.messages.back().transmitterId;
}


void MainWidget::makeContactList() {
    for (auto it = contactList->list.begin(); it != contactList->list.end(); ++it) {
//        std::cout << JsonParser::jsonDataToJson(*it) << std::endl;
        dialogMap->contactWidget[dialogId(*it)] = std::make_unique<ContactWidget>(*it, contactListWidget.get());
    }
}


void MainWidget::addMessageToDialog(JsonData& message) {
    std::string str;

    str.append("transmitterId #");
    str.append(std::to_string(message.messages.back().transmitterId));
    str.append(" <");

//    auto t = std::time_t(std::stol(message.messages.back().date));
//    char timestr[100];
//    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&t));

    std::string timestr = message.messages.back().date;

    str.append(timestr);
    str.append("> \n");
    str.append(message.messages.back().text);
    str.append("\n");

    QLabel* label = new QLabel(QString::fromStdString(str));
    QListWidgetItem* messageItem = new QListWidgetItem(dialogWidget.get());
    messageItem->setSizeHint(label->sizeHint());
    if (message.messages.back().transmitterId == sessionInformation.users.back().userId) {
        label->setAlignment(Qt::AlignRight);
    }
    dialogWidget.get()->setItemWidget(messageItem, label);

    dialogMessagePtr->add(label);
    dialogMessagePtr->add(messageItem);

    dialogWidget->scrollToBottom();
}


void MainWidget::addVoiceMessageToDialog(JsonData& message) {
    std::string str;
    str.append("transmitterId #");
    str.append(std::to_string(message.messages.back().transmitterId));
    str.append(" <");

//    auto t = std::time_t(std::stol(message.messages.back().date));
//    char timestr[100];
//    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&t));
    std::string timestr = message.messages.back().date;


    str.append(timestr);
    str.append("> \n");
    str.append(message.messages.back().text);
    str.append("\n");

    QListWidgetItem* messageItem = new QListWidgetItem(dialogWidget.get());
    dialogMessagePtr->add(messageItem);
    QWidget* messageWidget = new QWidget(dialogWidget.get());
    dialogMessagePtr->add(messageWidget);
    QHBoxLayout* messageLayout = new QHBoxLayout(messageWidget);
    dialogMessagePtr->add(messageLayout);
    messageWidget->setLayout(messageLayout);
    messageLayout->setMargin(0);

    QWidget* leftSpacer = new QWidget(dialogWidget.get());
    dialogMessagePtr->add(leftSpacer);
    messageLayout->addWidget(leftSpacer, 7);

    QLabel* label = new QLabel(QString::fromStdString(str), dialogWidget.get());
    dialogMessagePtr->add(label);
    messageLayout->addWidget(label, 2);
    PlayButton* playButton = new PlayButton(dialogWidget.get());
    dialogMessagePtr->add(playButton);
    playButton->setText("&Play");

    playButton->fileName = message.messages.back().text;
    messageLayout->addWidget(playButton, 1);

    connect(playButton, SIGNAL(clicked()),
            this, SLOT(play()));

    QPushButton* stopButton = new QPushButton(dialogWidget.get());
    dialogMessagePtr->add(stopButton);
    stopButton->setText("&Stop");
    messageLayout->addWidget(stopButton, 1);
    stopButton->hide();

    connect(stopButton, SIGNAL(clicked()),
            this, SLOT(stop()));

    messageItem->setSizeHint(messageWidget->sizeHint());
    dialogWidget->setItemWidget(messageItem, messageWidget);

    QWidget* rightSpacer = new QWidget(dialogWidget.get());
    dialogMessagePtr->add(rightSpacer);
    messageLayout->addWidget(rightSpacer, 7);

    if (dialogId(selectedContact->back()) == message.messages.back().receiverId) {
        rightSpacer->hide();
    } else {
        leftSpacer->hide();
    }

    connect(playButton, SIGNAL(clicked()), playButton, SLOT(hide()));
    connect(playButton, SIGNAL(clicked()), stopButton, SLOT(show()));
    connect(playButton, SIGNAL(clicked(bool)), stopButton, SLOT(setDisabled(bool)));

    connect(stopButton, SIGNAL(clicked()), stopButton, SLOT(hide()));
    connect(stopButton, SIGNAL(clicked()), playButton, SLOT(show()));


    dialogWidget->scrollToBottom();
}


void MainWidget::makeDialog(QListWidgetItem* item) {
    dialogMessagePtr->clear();
    bottomBoardWidget->show();
    dialogWidget->clear();

    int j = 0;
    while(contactListWidget->item(j) != item) {
        ++j;
    }
    auto it = contactList->list.begin();
    for (int i = 0; i < j; ++it, ++i);

    selectedContact = &dialogMap->map[dialogId(*it)];

    if (selectedContact->empty()) {
        return;
    }

    //TODO если выбран этот диалог, то при добавлении в него сообщения не нужно
    //отрисовывать по новой все сообщения

    for (auto &it : *selectedContact) {
        if (it.messages.back().contentType == "audio") {
            addVoiceMessageToDialog(it);
        } else {
            if (it.messages.back().text.empty()) {
                continue;
            }
            addMessageToDialog(it);
        }
    }
}


void MainWidget::sendMessage() {
    if (messageTextBrowser->toPlainText().isEmpty()) {
        return;
    }
    JsonData message;
    MessageData mess;

    message.users.push_back(UserData());
    message.users.back().userId = sessionInformation.users.back().userId;
    message.users.back().name = sessionInformation.users.back().name;

    message.requestType = "message";
    message.requestStatus = 1;
    message.errorDescription = 0;

    mess.transmitterId = sessionInformation.users.back().userId;
    mess.receiverId = dialogId(selectedContact->back());
//    mess.date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    auto temp = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&temp));
    mess.date = timestr;


    mess.text = messageTextBrowser->toPlainText().toUtf8().constData();
    mess.contentType = "text";
    mess.chatType = "private";
    message.messages.push_back(mess);

    sendMessage(message);
}


void MainWidget::sendMessage(JsonData& message) {

    message.messages.back().transmitterId = sessionInformation.users.back().userId;
    message.messages.back().receiverId = dialogId(selectedContact->back());

//    message.users.back().name = selectedContact->back().users.back().name;

//    auto messageStr = JsonParser::jsonDataToJson(message);

    auto reply = client->write_message(message);

//    std::cout << JsonParser::jsonDataToJson(message) << std::endl;

//    std::string replyStr = "";
//    if (!serverConnection.start()) {
//        replyStr = serverConnection.echoWriteRead(messageStr);
//    }
//    serverConnection.stop();

//    if (replyStr != messageStr) {
//        std::cout << "message not delivered" << std::endl;
//        message.messages.back().text = "<not delivered> | " + message.messages.back().text;
//    } else {
//        message.requestStatus = 1;
//        std::cout << "message was sended" << std::endl;
//    }

        if (reply.errorDescription) {
            std::cout << "message not delivered" << std::endl;
            message.messages.back().text = "<not delivered> | " + message.messages.back().text;
        } else {
            message.requestStatus = 1;
            std::cout << "message was sended" << std::endl;
        }

    selectedContact->push_back(message);

    if (message.messages.back().contentType == "audio") {
        addVoiceMessageToDialog(message);
        return;
    }

    addMessageToDialog(message);
    messageTextBrowser->clear();

    for (auto &it: dialogMap->contactWidget) {
        it.second->update(dialogMap->map[it.first].back());
    }

}


void MainWidget::sendFile(const std::string filePath) {
    QFile f(QString::fromStdString(filePath));
    if (!f.exists()) {
        std::cout << "bruh" << std::endl;
        return;
    }

//    if (!f.open(QIODevice::ReadOnly)) {
//        std::cout << "bruh2" << std::endl;
//        return;
//    }

        client->sendFile(filePath);


}


void MainWidget::receiveAudio(JsonData data) {

    client->fileName = "audio/" + data.messages.back().text;



//    QFile f(QString::fromStdString("testAudio"));
//    if (!f.open(QIODevice::ReadOnly)) {
//        return;
//    }

//    if (serverConnection.start()) {
//        serverConnection.stop();
//        std::cout << "connection error" << std::endl;
//        return;
//    }

//    std::cout << std::to_string(f.size()) << std::endl;

//    std::string fileStr(f.read(f.size()).constData(), f.size());

//    std::string receiveFileStr =
//        serverConnection.echoWriteReadFile(std::to_string(f.size()), fileStr);
//    serverConnection.stop();
//    f.close();

////-----------------------
//    QFile r("audio/temp");
//    QString temp = "audio/" + QString::fromStdString(data.messages.back().text);
//    r.rename(temp);
//    r.close();
}


void MainWidget::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Escape) {
        selectedContact = nullptr;
        contactListWidget->clearSelection();

        dialogMessagePtr->clear();
        messageTextBrowser->clear();

        bottomBoardWidget->hide();
        dialogWidget->clear();
        return;
    }
}


void TextEdit::keyPressEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Enter) {
        if (e->modifiers() & Qt::ControlModifier) {
            append("");
            return;
        }
        auto temp = static_cast<MainWidget*>(mainWindow);
        temp->sendMessage();
        return;
    }
    QTextEdit::keyPressEvent(e);
}


void MainWidget::recordAudio() {
    std::cout << "record audio" << std::endl;

    disconnect(contactListWidget.get(), SIGNAL(itemClicked(QListWidgetItem*)),
               this, SLOT(makeDialog(QListWidgetItem*)));

    contactListWidget->setEnabled(false);
    recordThread("audio/1.wav").record();

}


void MainWidget::stopRecordAudio() {


    recordThread.stop();

    connect(contactListWidget.get(), SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(makeDialog(QListWidgetItem*)));
    contactListWidget->setEnabled(true);

    JsonData data;
    MessageData messData;
    data.users.push_back(UserData());
    data.users.back().userId = sessionInformation.users.back().userId;
    messData.transmitterId = sessionInformation.users.back().userId;
    messData.receiverId = dialogId(selectedContact->back());
//    messData.date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));



    auto temp2 = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&temp2));
    messData.date = timestr;


    messData.contentType = "audio";
    messData.text = "";
    messData.text.append(std::to_string(data.users.back().userId));
    messData.text.append(" -> ");
    messData.text.append(std::to_string(dialogId(selectedContact->back())));
    messData.text.append("|");
    data.messages.push_back(messData);



//    auto t = std::time_t(std::stol(data.messages.back().date));
//    std::string timestr = data.messages.back().date;

    std::string timestr2 = data.messages.back().date;

//    char timestr[100];
//    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&t));

    data.messages.back().text.append(timestr2);
    data.messages.back().text.append(".wav");
    data.requestType = "message";

    QFile f("audio/1.wav");
    QString temp = "audio/" + QString::fromStdString(data.messages.back().text);
    f.rename(temp);
    f.close();

    sendMessage(data);

    std::cout << temp.toStdString() << std::endl;
    sendFile(temp.toStdString());
}


void MainWidget::play() {
    std::string fileName = "audio/";
    fileName.append(static_cast<PlayButton*>(sender())->fileName);

    auto temp = dialogWidget->children();
    for (auto &it : temp) {
      auto t = it->parent()->findChildren<PlayButton*>();
      for (auto &itt : t) {
          if(itt->fileName != "") {
              itt->setDisabled(true);
          }
      }
    }

    disconnect(contactListWidget.get(), SIGNAL(itemClicked(QListWidgetItem*)),
               this, SLOT(makeDialog(QListWidgetItem*)));

    contactListWidget->setEnabled(false);
    soundThread(QString::fromStdString(fileName)).start();
}


void MainWidget::stop() {
    auto temp = dialogWidget->children();
    for (auto &it : temp) {
      auto t = it->parent()->findChildren<PlayButton*>();
      for (auto &itt : t) {
          if(itt->fileName != "") {
              itt->setDisabled(false);
          }
      }
    }

    connect(contactListWidget.get(), SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(makeDialog(QListWidgetItem*)));

    contactListWidget->setEnabled(true);
    soundThread.stop();
}


void MainWidget::receiveMessage(QString messageStr) {
//    if (serverConnection.start()) {
//        std::cout << "no connection" << std::endl;
//        return;
//    }

//    std::string replyStr = serverConnection.echoWriteRead(messageStr);
//    serverConnection.stop();

//    auto reply = JsonParser::jsonToJsonData(replyStr);

    auto input = messageStr.toStdString();

    if (input.empty()) {
        return;
    }

    auto reply = JsonParser::jsonToJsonData(input);

    dialogMap->addMessage(reply);
    if (reply.messages.back().contentType == "audio") {
        receiveAudio(reply);
    }



//    std::cout << input << std::endl;

    auto selectedItems = contactListWidget->selectedItems();
    int idx = 0;

    if (!selectedItems.isEmpty()) {
        while (contactListWidget.get()->item(idx) != selectedItems.first()) {
            idx++;
        }
    }


    std::size_t dialogId;
    if (!selectedItems.isEmpty()) {
        auto temp = contactList->list.begin();
        for (int j = 0; j < idx; ++j) {
            ++temp;
        }
        dialogId = temp->messages.back().transmitterId;
    }



    contactList->add(reply);

    dialogMap->clearContactWidget();
    contactListWidget->clear();
    makeContactList();



    if (!selectedItems.isEmpty()) {
        int k = 0;
        for (auto &it: contactList->list) {
            if (it.messages.back().transmitterId == dialogId) {
                break;
            }
            ++k;
        }
        auto temp = contactListWidget.get()->item(k);
        temp ->setSelected(true);
        makeDialog(temp);
    }

    for (auto &it: dialogMap->contactWidget) {
        it.second->update(dialogMap->map[it.first].back());
    }
}



void MainWidget::testSlot() {
    std::cout << std::endl;
    std::cout << "testSlot" << std::endl;

    std::cout << "selected contact: " << std::endl;
    std::cout << JsonParser::jsonDataToJson(selectedContact->back()) << std::endl;

//    addContact();
//    return;

//    --------------------

//    JsonData data;
//    data.transmitterId = 7555;

//    data.date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

//    data.text = "message #";
//    data.text.append(std::to_string(23));
//    data.name = "contact #";
//    data.name.append(std::to_string(7555));
//    data.receiverId = 7815;

//    auto temp = JsonParser::jsonDataToJson(data);
//    receiveMessage(temp);

//    --------------------

//    writeCache(*dialogMap);

//    ----------------------------

//        JsonData data;
//        data.users.push_back(UserData());
//        data.messages.push_back(MessageData());
//        data.users.back().userId = 213;
//        data.messages.back().receiverId = sessionInformation.users.back().userId;
//        data.messages.back().transmitterId = 213;
//        data.users.back().name = "contact #" + std::to_string(data.users.back().userId);
//        data.messages.back().date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
////        data.messages.back().contentType = "audio";
//        data.messages.back().contentType = "text";
//        data.messages.back().text = "";
//        data.messages.back().text.append(std::to_string(data.users.back().userId));
//        data.messages.back().text.append(" -> ");
//        data.messages.back().text.append(std::to_string(sessionInformation.users.back().userId));
//        data.messages.back().text.append("|");

//        auto t = std::time_t(std::stol(data.messages.back().date));
//        char timestr[100];
//        std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&t));

//        data.messages.back().text.append(timestr);
//        data.messages.back().text.append(".wav");

//        auto temp = JsonParser::jsonDataToJson(data);
//        receiveMessage(temp);

//        ----------------------------

//    if (!serverConnection.start()) {
//        std::cout << "no connection" << std::endl;
//        serverConnection.stop();
//        return;
//    }

//    auto data = JsonParser::jsonDataToJson(sessionInformation);
//    serverConnection.write(data);

//        ===========================================

//    for (auto &it: dialogMap->map) {
//        std::cout << it.first << " " << it.second.back().users.back().userId << std::endl;
//    }


//      std::cout << "session information: " <<
//                   sessionInformation.users.back().userId << std::endl;


    //    //    ===========================================

    //        std::cout << "bruh" << std::endl;
    //        std::cout << JsonParser::jsonDataToJson(message) << std::endl;

    //    //    ===========================================


//        //    ===========================================

//            for (auto &it: dialogMap->map) {
//                for (auto &mit: it.second) {
//            std::cout << it.first << ": " <<
//                      JsonParser::jsonDataToJson(mit) << std::endl;
//                }
//            }

//        //    ===========================================

//    //    ===========================================

//        for (auto &it: contactList->list) {
//            std::cout << JsonParser::jsonDataToJson(it) << std::endl;
//        }

//    //    ===========================================

//        JsonData data;
//        data.messages.push_back(MessageData());
//        data.users.push_back(UserData());

//        data.messages.back().transmitterId = 7555;

//        data.messages.back().date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

//        data.messages.back().text = "message #";
//        data.messages.back().text.append(std::to_string(23));
//        data.users.back().name = "contact #";
//        data.users.back().name.append(std::to_string(7555));
//        data.messages.back().receiverId = 119;

//        auto temp = JsonParser::jsonDataToJson(data);
//        receiveMessage(temp);

}


void MainWidget::addContact() {
    selectedContact = nullptr;
    contactListWidget->clearSelection();

    dialogMessagePtr->clear();
    messageTextBrowser->clear();

    bottomBoardWidget->hide();
    dialogWidget->clear();

    newDialogWidget->show();
    newDialogWidget->readSessionInformation(sessionInformation);
}


void MainWidget::createNewDialog(QString text) {
    std::cout << "createNewDialog" << std::endl;

    dialogMessagePtr->clear();
    bottomBoardWidget->show();
    dialogWidget->clear();

    auto temp = JsonParser::jsonToJsonData(text.toStdString());

    temp.messages.push_back(MessageData());

//            std::cout << text.toStdString() << std::endl;

    temp.messages.back().receiverId = sessionInformation.users.back().userId;
    temp.messages.back().transmitterId = temp.users.back().userId;
//    temp.messages.back().date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));

    auto temp1 = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&temp1));
    temp.messages.back().date = timestr;


    int j = 0;
    for (auto &it: contactList.get()->list) {
        if (dialogId(it) == dialogId(temp)) {
            contactListWidget.get()->item(j)->setSelected(true);
            makeDialog(contactListWidget.get()->item(j));
            messageTextBrowser->setFocus();
            return;
        }
        j++;
    }


    dialogMap->addMessage(temp);
    selectedContact = &dialogMap->map[dialogId(temp)];
    contactList->add(temp);

//    for (auto &it: contactList->list) {
//        std::cout << JsonParser::jsonDataToJson(it) << std::endl;
//    }

    dialogMap->clearContactWidget();
    contactListWidget->clear();
    makeContactList();

    for (auto &it: dialogMap->contactWidget) {
        it.second->update(dialogMap->map[it.first].back());
    }

    int idx = 0;
    for (auto &it: contactList.get()->list) {
        if (dialogId(it) == dialogId(temp))
            break;
        idx++;
    }
    contactListWidget.get()->item(idx)->setSelected(true);
    messageTextBrowser->setFocus();
}


void MainWidget::writeCache(DialogMapClass &dialogMap) {
    QDir dir("cache/archive");
    QFile f(dir.path() + "/" + QString::fromStdString(sessionInformation.users.back().login));
    if (!f.open(QIODevice::WriteOnly)) {
        return;
    }
    for (auto &mit: dialogMap.map) {
        for (auto &dit: mit.second) {
            auto temp = JsonParser::jsonDataToJson(dit);
            QByteArray byteArray(temp.c_str(), temp.length());
            f.write(byteArray);
        }
    }
    f.close();
}

static std::size_t endJson(const std::string& str) {
    std::list<int> list;
    size_t i = 0;
    for (auto &it: str) {
        ++i;
        if (it == '{') {
            list.push_back(1);
        }
        if (it == '}') {
            list.pop_back();
            if (list.empty()) {
                break;
            }
        }
    }
    return i;
}

//static

void MainWidget::handleAuthorizeReply(JsonData data) {
    if (data.messages.empty()) {
        return;
    }
    for (auto it: data.messages) {

//        ----------------------------
        if (it.contentType == "audio") {

            QFile f("audio/" + QString::fromStdString(data.messages.back().text));
            if (!f.exists()) {


                std::cout << "file are misssed" << std::endl;

                JsonData newData;
                newData.messages.push_back(MessageData());
                newData.messages.back().text = it.text;
                newData.requestType = "file_required";

                client->fileName = "audio/" + newData.messages.back().text;

                auto reply = client->write_message(newData);
                std::cout << JsonParser::jsonDataToJson(reply) << std::endl;

            }
        }


//        ----------------------------




        JsonData temp = data;
        temp.messages.clear();
        temp.messages.push_back(it);

        dialogMap->addMessage(temp);
        contactList->add(temp);
    }

    makeContactList();
    for (auto &it: dialogMap->contactWidget) {
        it.second->update(dialogMap->map[it.first].back());
    }
}


void MainWidget::readCache() {
//    dialogMap->readSessionInformation(sessionInformation);

    QDir dir("cache/archive");
    QFile f(dir.path() + "/" + QString::fromStdString(sessionInformation.users.back().login));

//    ----------------------------------

// если файла не существует - запросить всю историю


//    ------------------------------------
    if (!f.open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray byteArray = f.read(1048576);
    f.close();

//    std::cout << std::string(byteArray) << std::endl;

//    ------------------------------


//    ------------------------------

    std::string str(byteArray.constData(), byteArray.length());
    while (true) {
        auto l = str.find('{');
        auto r = endJson(str);

        if (l == std::string::npos)
            break;

        std::string temp(str, l, r);
        JsonData data = JsonParser::jsonToJsonData(temp);


////        ----------------------------
//        if (data.messages.back().contentType == "audio") {

//            QFile f("audio/" + QString::fromStdString(data.messages.back().text));
//            if (!f.exists()) {


//                std::cout << "file are misssed" << std::endl;

//                JsonData newData;
//                newData.messages.push_back(MessageData());
//                newData.messages.back().text = data.messages.back().text;
//                newData.requestType = "file_required";

//                client->fileName = "audio/" + newData.messages.back().text;

//                auto reply = client->write_message(newData);
//                std::cout << JsonParser::jsonDataToJson(reply) << std::endl;

//            }
//        }


////        ----------------------------

        dialogMap->addMessage(data);
        contactList->add(data);

        str = std::string(str, r + 1);
    }

//    ------------------------------

    makeContactList();
    for (auto &it: dialogMap->contactWidget) {
        it.second->update(dialogMap->map[it.first].back());
    }

//    for (auto &it: contactList->list) {
//        std::cout << it.messages.back().transmitterId << " "
//        << it.messages.back().receiverId << ": "
//        << it.messages.back().text << std::endl;
//    }
}


void MainWidget::readSessionInformation(JsonData data) {
    sessionInformation = data;
}


void MainWidget::closeEvent(QCloseEvent*) {
    QFile f("cache/archive/" + QString::fromStdString(sessionInformation.users.back().login));
    f.remove();
    writeCache(*dialogMap);

    sessionInformation.requestType = "deauthorize";
    client->write_message(sessionInformation);

    newDialogWidget->close();
    this->close();
}


MainWidget::~MainWidget()
{
}

