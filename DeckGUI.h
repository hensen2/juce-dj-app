/*
  ==============================================================================

    DeckGUI.h
    Created: 14 Jul 2022 3:05:21pm
    Author:  Matt

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
*/
class DeckGUI  : public Component,
    public Button::Listener,
    public Slider::Listener,
    public FileDragAndDropTarget,
    public Timer
{
public:
    DeckGUI(DJAudioPlayer* player, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    void paint (Graphics&) override;
    void resized() override;

    //Event Listeners
    void buttonClicked(Button* button) override;
    void sliderValueChanged(Slider* slider) override;

    // Drag and drop files
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void timerCallback() override;

private:

    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "STOP" };
    TextButton loadButton{ "LOAD" };

    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    FileChooser fChooser{ "Select a file..." };

    WaveformDisplay waveformDisplay;

    DJAudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
