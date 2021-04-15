#pragma once

#ifndef LIB_H
#define LIB_H

#include <QWidget>
#include <QWidget>
#include <QAudioRecorder>
#include <QAudioOutput>
#include <QAudio>
#include <QFile>

#include "jsonParser.h"
#include "jsonData.h"
#include "autorizwindow.h"
#include "registrationwindow.h"
#include "waitwindow.h"

#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/ServerConnection.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <cstddef>
#include <iostream>
#include <time.h>

class ServerConnection{
private:
    int ServerConnectiondes;
    struct sockaddr_in* name;
    const char* hostname;
    uint16_t port;

    class jsonParser parcer;

public:
    ServerConnection(const char* hostname, uint16_t port);
    ~ServerConnection();
    ServerConnection(ServerConnection&& other) = delete;
    ServerConnection& operator=(ServerConnection&& other) = delete;

//    void sendToServer(std::byte binaryArr);  // !!!
    void sendToServer(std::string json);
    std::string getFromServer();
};


class AudioRecorder: public QAudioRecorder {
private:
    QAudioEncoderSettings encoderSettings;
    QString container;
    QString fileName;

public:
    AudioRecorder();  //private members initialization
    ~AudioRecorder();

    void toContainer();
};

class AudioOutput: public QAudioOutput {
private:
    QAudioFormat audioFormatSettings;

public:
    AudioOutput(QFile fileName);
    ~AudioOutput();

    void extractFromContainer();
};

class PullThread {
private:
    pthread_t thread;
    void* arg;
    ServerConnection sock;

public:
    PullThread(void* arg = nullptr);
    ~PullThread();

    void routine();
};

class RegistationRequest: public JsonData {
public:
    RegistationRequest(std::string name,
                       std::string surname,
                       int age,
                       std::string login,
                       std::string password);
    ~RegistationRequest();
};

class CurrentSessionData: public JsonData {
public:
    CurrentSessionData(std::string cookiePath);  // read inf about user from cookie
    ~CurrentSessionData();
};

class Message: public JsonData {
public:
    Message(QString text, CurrentSessionData* sessionData);  // output mess generation
    Message(std::string text);  // input mess generation
    ~Message();
};

class AuthorizationRequest: public JsonData {
public:
    AuthorizationRequest(CurrentSessionData* sessionData);
    AuthorizationRequest(std::string login, std::string password);
    ~AuthorizationRequest();
};

class PullRequest: public JsonData {
public:
    PullRequest(CurrentSessionData* sessionData);
    ~PullRequest();
};

class ArchiveLoadRequest: public JsonData {
public:
    ArchiveLoadRequest(CurrentSessionData* sessionData);
    ~ArchiveLoadRequest();
};



class MainWindow: public QWidget {
    Q_OBJECT

private:
    QFile audioFile;
    PullThread* pullThread = nullptr;
    AudioRecorder* audioRecorder = nullptr;
    AudioOutput* audioOutput = nullptr;

    AutorizWindow* autorizWindow = nullptr;
    RegistrationWindow* registrWindow = nullptr;
    WaitWindow* waitWindow = nullptr;

    QWidget* dialogsList = nullptr;
    QWidget* currentDialog = nullptr;
    QWidget* messageField = nullptr;

    QWidget* MainWindowWidget[];

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    MainWindow(const MainWindow& other) = delete;
    MainWindow(MainWindow&& other) = delete;
    MainWindow& operator=(const MainWindow& other) = delete;
    MainWindow& operator=(MainWindow&& other) = delete;


    void updateInterface();
};

std::string passwordHashGenerate(std::string password);
void createAudioFile(std::byte byteArr);

template <typename T>
void addToCache(T* data);

void serverResponceHandler(JsonData* serverResponce);



#endif // LIB_H










