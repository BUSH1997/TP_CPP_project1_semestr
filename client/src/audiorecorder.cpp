#include "audiorecorder.h"
#include <QUrl>

AudioRecorder::AudioRecorder(QString fileName) {

    encoderSettings.setSampleRate(48000);
    encoderSettings.setChannelCount(1);
    encoderSettings.setBitRate(48000 * 1 * 16);
    encoderSettings.setCodec(codec);

    audioRecorder = new QAudioRecorder;

    audioRecorder->setEncodingSettings(encoderSettings, QVideoEncoderSettings(), container);
    audioRecorder->setOutputLocation(QUrl::fromLocalFile(fileName));

}

void AudioRecorder::record() {
    audioRecorder->record();
}

void AudioRecorder::stop() {
    audioRecorder->stop();
}

int AudioRecorder::testRecord() {
    audioRecorder->record();

    int ms = 5000;
    struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
    nanosleep(&ts, NULL);

    audioRecorder->stop();
    return 0;
}

AudioRecorder::~AudioRecorder() {
    delete audioRecorder;
}


