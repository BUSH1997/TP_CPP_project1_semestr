#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QAudioRecorder>
#include <iostream>

#include <QAudioEncoderSettings>
#include <QUrl>
#include <QTimer>

class AudioRecorder {
private:
    QAudioRecorder* audioRecorder;
    QAudioEncoderSettings encoderSettings;
    QString container = "audio/x-wav";
    QString codec = "audio/x-raw";
    QString fileName = "test2.wav";


public:
    AudioRecorder(QString fileName);  //private members initialization
    ~AudioRecorder();

    AudioRecorder(const AudioRecorder& other) = delete;
    AudioRecorder(AudioRecorder&& other) = delete;
    AudioRecorder& operator=(const AudioRecorder& other) = delete;
    AudioRecorder& operator=(AudioRecorder&& other) = delete;

    void toContainer();
    int testRecord();

    void record();
    void stop();
};
#endif // AUDIORECORDER_H
