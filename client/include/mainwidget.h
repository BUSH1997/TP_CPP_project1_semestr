#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QDir>
#include <QGuiApplication>
#include <QScreen>
#include <QLayout>
#include <QPushButton>
#include <QObject>
#include <QKeyEvent>
#include <QTextEdit>
#include <QThread>
#include <QLabel>

#include <ctime>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <memory>

#include "audiooutput.h"
#include "audiorecorder.h"
#include "parser.h"
#include "contactWidget.h"
#include "serverConnection.h"
#include "newdialogwidget.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <cstddef>

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

#include <fstream>

class ContactListClass {
private:
    JsonData sessionInformation;

    std::size_t& dialogId(JsonData& data) {
        if (data.messages.back().transmitterId == sessionInformation.users.back().userId) {
            return data.messages.back().receiverId;
        }
        return data.messages.back().transmitterId;
    }

public:
    std::list<JsonData> list;

public:
    ContactListClass(JsonData& sessionInformation) :
        sessionInformation(sessionInformation) {};

    void add(JsonData data) {
        for(auto it = list.begin(); it != list.end(); ++it) {
            if (dialogId(*it) == dialogId(data)) {
                list.erase(it);
                break;
            }
        }
        list.push_front(data);

        list.sort([](JsonData a, JsonData b) {
            return a.messages.back().date > b.messages.back().date;
        });
    }

    void readSessionInformation(JsonData& data) {
        this->sessionInformation = data;
    }

};


class DialogMapClass {
private:
    JsonData sessionInformation;

    std::size_t& dialogId(JsonData& data) {
        if (data.messages.back().transmitterId == sessionInformation.users.back().userId) {
            return data.messages.back().receiverId;
        }
        return data.messages.back().transmitterId;
    }

public:
    DialogMapClass(JsonData& sessionInformation) :
        sessionInformation(sessionInformation) {};

    std::map<unsigned long int, std::list<JsonData>> map;
    std::map<unsigned long int, std::unique_ptr<ContactWidget>> contactWidget;

    void addMessage(JsonData data) {
        map[dialogId(data)].push_back(data);  // могут приходить не одновременно

        map[dialogId(data)].sort([](JsonData a, JsonData b) {
            return a.messages.back().date < b.messages.back().date;
        });
    }

    void clearContactWidget() {
        contactWidget.clear();
    }

    void readSessionInformation(JsonData data) {
        this->sessionInformation = data;
    }
};


class TextEdit: public QTextEdit {
private:
    void* mainWindow = nullptr;
public:
    TextEdit(QWidget* parent = nullptr, void* mainWindow = nullptr) {
        this->mainWindow = mainWindow;
        this->setParent(parent);
    }
protected:
    void keyPressEvent(QKeyEvent *e);
};


class RecordThread: public QThread {
private:
    AudioRecorder* rec;
public:
    void record() {
         rec->record();
    }

    void stop() {
        rec->stop();
        delete rec;
    }

    RecordThread() {
        std::cout << "thread was created" << std::endl;
    }

    RecordThread& operator()(QString fileName) {
        rec = new AudioRecorder(fileName);
        return *this;
    }

    ~RecordThread() {
        quit();
        wait();
    }
};


class SoundThread: public QThread {

public:
    QAudioOutput* audio;

private:
    QAudioFormat audioFormat;
    QFile sourceFile;
    QString fileName = "./tests/test2.wav";
    QString codec = "audio/x-raw";

public:
    void start() {
        sourceFile.open(QIODevice::ReadOnly);
        audio = new QAudioOutput(audioFormat);
        audio->setVolume(50);
        audio->start(&sourceFile);
    }
    void stop() {
        audio->stop();
        sourceFile.close();
        delete audio;
    }

    SoundThread& operator()(QString fileName) {
        if (sourceFile.isOpen()) {
            audio->stop();
        }

        sourceFile.setFileName(fileName);

        //TODO handle this
        return *this;
    }

    SoundThread() {
        audioFormat.setSampleRate(48000);
        audioFormat.setChannelCount(1);
        audioFormat.setSampleSize(16);
        audioFormat.setCodec(codec);
        audioFormat.setByteOrder(QAudioFormat::LittleEndian);
        audioFormat.setSampleType(QAudioFormat::SignedInt);


        std::cout << "soundThread was created" << std::endl;
    }


    ~SoundThread() {
        quit();
        wait();
    }

};


class PlayButton: public QPushButton {
public:
    PlayButton(QWidget* parent) {
        this->setParent(parent);
    }

    ~PlayButton() = default;


    std::string fileName = "";
};


class DialogMessagePtr {
public:
    std::list<QObject*> list;
    std::list<QListWidgetItem*> itemList;

    void add(QObject* ptr) {
        list.push_front(ptr);
    }

    void add(QListWidgetItem* ptr) {
        itemList.push_front(ptr);
    }

    void clear() {
        if (!list.empty()) {
            for (auto &it : list) {
                delete it;
            }
            list.clear();
        }

        if (!itemList.empty()) {
            for (auto &it : itemList) {
                delete it;
            }
            itemList.clear();
        }
    }

    DialogMessagePtr() = default;
    ~DialogMessagePtr() {
        clear();
    };

};


//================================
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void makeContactList();
    void readSessionInformation(JsonData data);

    void sendFile(const std::string filePath);

    void handleAuthorizeReply(JsonData);

    std::size_t& dialogId(JsonData& data);

    std::unique_ptr<ContactListClass> contactList;
    std::unique_ptr<DialogMapClass> dialogMap;

    std::unique_ptr<QLineEdit> inputMessage;
    std::string fileName;

protected:
    void keyPressEvent(QKeyEvent *e);
    void closeEvent(QCloseEvent*);

public slots:
    void makeDialog(QListWidgetItem* item);
    void addMessageToDialog(JsonData& message);
    void addVoiceMessageToDialog(JsonData& message);
    void sendMessage();
    void sendMessage(JsonData& message);
//    void receiveMessage(std::string& messageStr);
    void receiveMessage(QString messageStr);
    void addContact();

    void recordAudio();
    void stopRecordAudio();

    void play();
    void stop();

    void testSlot();

    void createNewDialog(QString text);

    void writeCache(DialogMapClass &dialogMap);
    void readCache();

    void receiveAudio(JsonData data);

private:
    //todo handle this
    std::list<JsonData>* selectedContact;
    JsonData sessionInformation;

    AudioRecorder* audioRecorder;
    RecordThread recordThread;
    SoundThread soundThread;

//    ServerConnection serverConnection;

    std::unique_ptr<NewDialogWidget> newDialogWidget;

//    --------------------
    std::unique_ptr<QWidget> mainWindowLayoutWidget;
    std::unique_ptr<QHBoxLayout> mainWindowLayout;
    std::unique_ptr<QWidget> rightBoardWidget;
    std::unique_ptr<QWidget> leftBoardWidget;
    std::unique_ptr<QVBoxLayout> rightBoardLayout;
    std::unique_ptr<QLabel> contactsLabel;
    std::unique_ptr<QPushButton> addContactButton;

//    --------------------
    std::unique_ptr<QPushButton> testFooButton;
    std::unique_ptr<QListWidget> contactListWidget;

//    --------------------
    std::unique_ptr<QVBoxLayout> leftBoardLayout;
    std::unique_ptr<QListWidget> dialogWidget;

//    --------------------
//    std::unique_ptr<DialogMapClass> dialogMap;

//    --------------------
    std::unique_ptr<QWidget> bottomBoardWidget;
    std::unique_ptr<QHBoxLayout> bottomBoardLayout;
    std::unique_ptr<TextEdit> messageTextBrowser;
    std::unique_ptr<QWidget> buttonsWidget;
    std::unique_ptr<QVBoxLayout> buttonsLayout;
    std::unique_ptr<QPushButton> sendButton;
    std::unique_ptr<QPushButton> recordButton;
    std::unique_ptr<QPushButton> stopButton;

//    --------------------
    std::unique_ptr<DialogMessagePtr> dialogMessagePtr;
    std::list<ContactWidget*> contactPtrList;

};

#endif // MAINWIDGET_H
