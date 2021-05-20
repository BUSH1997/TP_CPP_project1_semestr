#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QAudioOutput>
#include <QFile>

class AudioOutput {
public:
    QAudioOutput* audio;

private:
    QAudioFormat audioFormat;
    QFile sourceFile;
    QString fileName = "./tests/test2.wav";
    QString codec = "audio/x-raw";


public:
    AudioOutput(QString fileName);
    ~AudioOutput();

    AudioOutput(const AudioOutput& other) = delete;
    AudioOutput(AudioOutput&& other) = delete;
    AudioOutput& operator=(const AudioOutput& other) = delete;
    AudioOutput& operator=(AudioOutput&& other) = delete;

    void extractFromContainer();

    void start();
    void stop();
};

#endif // AUDIOOUTPUT_H
