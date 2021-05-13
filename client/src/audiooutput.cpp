#include "audiooutput.h"

AudioOutput::AudioOutput(QString fileName) {

    sourceFile.setFileName(fileName);
    sourceFile.open(QIODevice::ReadOnly);
    //TODO handle this

    audioFormat.setSampleRate(48000);
    audioFormat.setChannelCount(1);
    audioFormat.setSampleSize(16);
    audioFormat.setCodec(codec);
    audioFormat.setByteOrder(QAudioFormat::LittleEndian);
    audioFormat.setSampleType(QAudioFormat::SignedInt);

    audio = new QAudioOutput(audioFormat);
    // TODO handle this

}

void AudioOutput::start() {
    audio->start(&sourceFile);
}


void AudioOutput::stop() {
    audio->stop();
}

void AudioOutput::test() {

}

AudioOutput::~AudioOutput() {
    delete audio;
}
