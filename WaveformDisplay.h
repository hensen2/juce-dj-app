/*
  ==============================================================================

    WaveformDisplay.h
    Created: 17 Jul 2022 5:52:29pm
    Author:  Matt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public Component,
    public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback(ChangeBroadcaster* source) override;

    void loadURL(URL audioURL);

    // set the relative position of the playhead
    void setPositionRelative(double pos);

private:

    AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
