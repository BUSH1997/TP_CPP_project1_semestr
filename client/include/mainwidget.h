#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtWidgets>
#include <ctime>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <memory>

#include "audiooutput.h"
#include "audiorecorder.h"
#include "Parser.h"
#include "contactWidget.h"
#include "serverConnection.h"

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


class ContactListClass {
public:
    std::list<JsonData> list;

public:
    void add(JsonData data) {
        for(auto it = list.begin(); it != list.end(); ++it) {

            std::cout << "data: " << data.transmitterId << std::endl;
            std::cout << "list: " << it->transmitterId << std::endl;
            std::cout << std::endl;

            if (it->transmitterId == data.transmitterId) {
                list.erase(it);
                break;
            }
        }
        list.push_front(data);

        list.sort([](JsonData a, JsonData b) {
            return a.date > b.date;
        });


    }

};


class DialogMapClass {
public:
    std::map<unsigned long int, std::list<JsonData>> map;
    std::map<unsigned long int, std::unique_ptr<ContactWidget>> contactWidget;

    void addMessage(JsonData data) {
        map[data.transmitterId].push_back(data);  // могут приходить не одновременно

        map[data.transmitterId].sort([](JsonData a, JsonData b) {
            return a.date < b.date;
        });
    }

    void clearContactWidget() {
        contactWidget.clear();
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

//    void makeContact(JsonData data);
    void makeContactList();

    void writeSessionInformation();
    void readSessionInformation();

    void sendFile(const std::string& filePath);

    int& dialogId(JsonData& data);

protected:
    void keyPressEvent(QKeyEvent *e);

public slots:
    void slotMakeDialog(QListWidgetItem* item);
    void addMessageToDialog(JsonData& message);
    void addVoiceMessageToDialog(JsonData& message);
    void sendMessage();
    void sendMessage(JsonData& message);
    void receiveMessage(std::string& messageStr);


    void recordAudio();
    void stopRecordAudio();

    void play();
    void stop();

    void testSlot();


private:

    std::list<JsonData>* selectedContact;


    JsonData sessionInformation;

    AudioRecorder* audioRecorder;

    RecordThread recordThread;
    SoundThread soundThread;



    ServerConnection serverConnection;





//    =========================
    std::unique_ptr<QWidget> mainWindowLayoutWidget;
    std::unique_ptr<QHBoxLayout> mainWindowLayout;
    std::unique_ptr<QWidget> rightBoardWidget;
    std::unique_ptr<QWidget> leftBoardWidget;
    std::unique_ptr<QVBoxLayout> rightBoardLayout;
    std::unique_ptr<QLabel> contactsLabel;

//    --------------------
    std::unique_ptr<QPushButton> testFooButton;
    std::unique_ptr<QListWidget> contactListWidget;
    std::unique_ptr<ContactListClass> contactList;

//    --------------------
    std::unique_ptr<QVBoxLayout> leftBoardLayout;
    std::unique_ptr<QListWidget> dialogWidget;

//    --------------------
    std::unique_ptr<DialogMapClass> dialogMap;

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
