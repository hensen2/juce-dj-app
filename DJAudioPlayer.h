/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Jul 2022 4:36:02pm
    Author:  Matt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class DJAudioPlayer : public AudioSource {
public:

    DJAudioPlayer(AudioFormatManager& formatManager);
    ~DJAudioPlayer();

    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    void start();
    void stop();

    // get the relative position of the playhead
    double getPositionRelative();

private:

    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};