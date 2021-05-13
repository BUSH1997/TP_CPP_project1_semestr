#include "mainwidget.h"


MainWidget::MainWidget(QWidget* parent): QWidget(parent) {
    QDir audioDir;
    audioDir.mkdir("audio");

    QDir cacheDir;
    cacheDir.mkdir("cache");

    readSessionInformation();

    dialogMessagePtr = std::make_unique<DialogMessagePtr>();

    resize(1366, 768);

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

//    ------------------------------
    testFooButton = std::make_unique<QPushButton>(rightBoardWidget.get());
    rightBoardLayout->addWidget(testFooButton.get(), 2);
    testFooButton.get()->setText("testFoo");
    connect(testFooButton.get(), SIGNAL(clicked()), this, SLOT(testSlot()));

    contactListWidget = std::make_unique<QListWidget>();
    rightBoardLayout->addWidget(contactListWidget.get(), 20);

    contactList = std::make_unique<ContactListClass>();
    connect(contactListWidget.get(), SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(slotMakeDialog(QListWidgetItem*)));

//    ------------------------------
    leftBoardLayout = std::make_unique<QVBoxLayout>();
    leftBoardWidget->setLayout(leftBoardLayout.get());

    dialogWidget = std::make_unique<QListWidget>();
    leftBoardLayout->addWidget(dialogWidget.get(), 7);
    dialogWidget->setSelectionMode(QAbstractItemView::NoSelection);

//    ---------------------
    dialogMap = std::make_unique<DialogMapClass>();

{
    for (int i = 0; i < 3; i++) {
        JsonData data;
        data.transmitterId = 0;

        data.date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() + std::chrono::hours(i)));

        data.text = "message #";
        data.text.append(std::to_string(i));
        data.name = "contact #";
        data.name.append(std::to_string(0));
        data.receiverId = 7815;

        dialogMap->addMessage(data);
        contactList->add(data);

    }

    for (int i = 0; i < 3; i++) {
        JsonData data;
        data.transmitterId = 4;

        data.date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() + std::chrono::hours(i)));

        data.text = "message #";
        data.text.append(std::to_string(i));
        data.name = "contact #";
        data.name.append(std::to_string(4));
        data.receiverId = 7815;

        dialogMap->addMessage(data);
        contactList->add(data);
    }


    JsonData data;
    data.transmitterId = 101;

    data.date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now() - std::chrono::hours(8)));

    data.text = "message #";
    data.text.append(std::to_string(10));
    data.name = "contact #";
    data.name.append(std::to_string(101));
    data.receiverId = 7815;

    dialogMap->addMessage(data);
    contactList->add(data);
}

    makeContactList();

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

    sendButton = std::make_unique<QPushButton>(buttonsWidget.get());
    buttonsLayout->addWidget(sendButton.get());
    sendButton->setText("&Send");
    connect(sendButton.get(), SIGNAL(clicked()), this, SLOT(sendMessage()));

    recordButton = std::make_unique<QPushButton>(buttonsWidget.get());
    buttonsLayout->addWidget(recordButton.get());
    recordButton->setText("&Record");
    connect(recordButton.get(), SIGNAL(clicked()), this, SLOT(recordAudio()));

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


int& MainWidget::dialogId(JsonData& data) {
    if (data.transmitterId == sessionInformation.userId) {
        return data.receiverId;
    }
    return data.transmitterId;
}

void MainWidget::makeContactList() {
    for (auto it = contactList->list.begin(); it != contactList->list.end(); ++it) {

//        dialogMap->contactWidget[it->transmitterId] = std::make_unique<ContactWidget>(*it, contactListWidget.get());
        dialogMap->contactWidget[dialogId(*it)] = std::make_unique<ContactWidget>(*it, contactListWidget.get());
    }
}


void MainWidget::addMessageToDialog(JsonData& message) {
    std::string str;

    str.append("transmitterId #");
    str.append(std::to_string(message.transmitterId));
    str.append(" <");

    auto t = std::time_t(std::stol(message.date));
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&t));

    str.append(timestr);
    str.append("> \n");
    str.append(message.text);
    str.append("\n");

    QLabel* label = new QLabel(QString::fromStdString(str));
    QListWidgetItem* messageItem = new QListWidgetItem(dialogWidget.get());
    messageItem->setSizeHint(label->sizeHint());
    if (message.transmitterId == sessionInformation.userId) {
        label->setAlignment(Qt::AlignRight);
    }
    dialogWidget.get()->setItemWidget(messageItem, label);

    dialogMessagePtr->add(label);
    dialogMessagePtr->add(messageItem);

    dialogWidget->scrollToBottom();
//    dialogMap->contactWidget[dialogId(message)]->update(message);

}


void MainWidget::addVoiceMessageToDialog(JsonData& message) {
    std::string str;

    str.append("transmitterId #");
    str.append(std::to_string(message.transmitterId));
    str.append(" <");

    auto t = std::time_t(std::stol(message.date));
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&t));

    str.append(timestr);
    str.append("> \n");
    str.append(message.text);
    str.append("\n");

    QListWidgetItem* messageItem = new QListWidgetItem(dialogWidget.get());
    dialogMessagePtr->add(messageItem);
    QWidget* messageWidget = new QWidget(dialogWidget.get());
    dialogMessagePtr->add(messageWidget);
    QHBoxLayout* messageLayout = new QHBoxLayout;
    dialogMessagePtr->add(messageLayout);
    messageWidget->setLayout(messageLayout);
    messageLayout->setMargin(0);


    QLabel* label = new QLabel(QString::fromStdString(str));
    dialogMessagePtr->add(label);
    messageLayout->addWidget(label, 2);
    PlayButton* playButton = new PlayButton(dialogWidget.get());
    dialogMessagePtr->add(playButton);
    playButton->setText("&Play");
    playButton->fileName = message.text;
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
    QWidget* spacer = new QWidget(dialogWidget.get());
    dialogMessagePtr->add(spacer);
    messageLayout->addWidget(spacer, 7);

    connect(playButton, SIGNAL(clicked()), playButton, SLOT(hide()));
    connect(playButton, SIGNAL(clicked()), stopButton, SLOT(show()));
    connect(playButton, SIGNAL(clicked(bool)), stopButton, SLOT(setDisabled(bool)));

    connect(stopButton, SIGNAL(clicked()), stopButton, SLOT(hide()));
    connect(stopButton, SIGNAL(clicked()), playButton, SLOT(show()));

}


void MainWidget::slotMakeDialog(QListWidgetItem* item) {
    dialogMessagePtr->clear();
    bottomBoardWidget->show();
    dialogWidget->clear();

    int j = 0;
    while(contactListWidget->item(j) != item) {
        ++j;
    }
    auto it = contactList->list.begin();
    for (int i = 0; i < j; ++it, ++i);

    selectedContact = &dialogMap->map[it->transmitterId];
    if (selectedContact->empty()) {
        return;
    }

    //TODO если выбран этот диалог, то при добавлении в него сообщения не нужно
    //отрисовывать по новой все сообщения

    for (auto &it : *selectedContact) {
        if (it.contentType == "audio") {
            addVoiceMessageToDialog(it);
        } else {
            addMessageToDialog(it);
        }
    }
}


void MainWidget::sendMessage() {
    if (messageTextBrowser->toPlainText().isEmpty()) {
        return;
    }


    JsonData message;
    message.transmitterId = sessionInformation.userId;
    message.receiverId = selectedContact->begin()->userId;
    message.date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    message.text = messageTextBrowser->toPlainText().toUtf8().constData();

    sendMessage(message);
}


void MainWidget::sendMessage(JsonData& message) {
    message.transmitterId = sessionInformation.userId;
//    //!!!

    auto messageStr = JsonParser::jsonDataToJson(message);

    std::string replyStr = "";
    if (!serverConnection.start()) {
        replyStr = serverConnection.echoWriteRead(messageStr);
    }
    serverConnection.stop();

    if (replyStr != messageStr) {
        std::cout << "message not delivered" << std::endl;
        message.text = "<not delivered> | " + message.text;
    } else {
        message.status = true;
        std::cout << "message was sended" << std::endl;
    }

    selectedContact->push_back(message);

    if (message.contentType == "audio") {
        addVoiceMessageToDialog(message);
        return;
    }

    addMessageToDialog(message);
    messageTextBrowser->clear();
}


void MainWidget::sendFile(const std::string& filePath) {

//    serverConnection.start();
//    serverConnection.sendFile(filePath);
//    serverConnection.stop();
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
    MainWidget::keyPressEvent(e);
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
               this, SLOT(slotMakeDialog(QListWidgetItem*)));

    contactListWidget->setEnabled(false);
    recordThread("audio/1.wav").record();
}


void MainWidget::stopRecordAudio() {
    recordThread.stop();

    connect(contactListWidget.get(), SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(slotMakeDialog(QListWidgetItem*)));
    contactListWidget->setEnabled(true);

    JsonData data;
    data.userId = sessionInformation.userId;
    data.date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    data.contentType = "audio";
    data.text = "";
    data.text.append(std::to_string(data.userId));
    data.text.append(" -> ");
    data.text.append(std::to_string(selectedContact->begin()->userId));
    data.text.append("|");

    auto t = std::time_t(std::stol(data.date));
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%F %T", std::localtime(&t));

    data.text.append(timestr);
    data.text.append(".wav");

    QFile f("audio/1.wav");
    QString temp = "audio/" + QString::fromStdString(data.text);
    f.rename(temp);

    sendMessage(data);
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
               this, SLOT(slotMakeDialog(QListWidgetItem*)));

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
            this, SLOT(slotMakeDialog(QListWidgetItem*)));

    contactListWidget->setEnabled(true);
    soundThread.stop();
}


void MainWidget::receiveMessage(std::string& messageStr) {
    if (serverConnection.start()) {
        std::cout << "no connection" << std::endl;
        return;
    }

    std::string replyStr = serverConnection.echoWriteRead(messageStr);
    serverConnection.stop();

    auto reply = JsonParser::jsonToJsonData(replyStr);

    dialogMap->addMessage(reply);

    auto selectedItems = contactListWidget->selectedItems();
    int idx = 0;
    if (!selectedItems.isEmpty()) {
        while (contactListWidget.get()->item(idx) != selectedItems.first()) {
            idx++;
        }
    }

    contactList->add(reply);
    dialogMap->clearContactWidget();
    contactListWidget->clear();
    makeContactList();

    if (!selectedItems.isEmpty()) {
        auto temp = contactListWidget.get()->item(idx);
        temp ->setSelected(true);
        slotMakeDialog(temp);
    }
}


void MainWidget::testSlot() {
    std::cout << "socket read" << std::endl;

    static int i = 0;
    JsonData message;
    message.transmitterId = 7;
    message.name = "contact #" + std::to_string(message.transmitterId);
    message.receiverId = sessionInformation.userId;
    message.date = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    message.text = "reply message #" + std::to_string(i++);
    std::string messageStr = JsonParser::jsonDataToJson(message);

    receiveMessage(messageStr);

    return;

}


void MainWidget::readSessionInformation() {
    QFile f("cache/sessionInformation");
    if (!f.open(QIODevice::ReadOnly)) {
        return;
    }
    QByteArray byteArray = f.read(1024);

    sessionInformation = JsonParser::jsonToJsonData(byteArray.toStdString());
}


MainWidget::~MainWidget()
{
}

