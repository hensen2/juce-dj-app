/*
  ==============================================================================

    DeckGUI.cpp
    Created: 14 Jul 2022 3:05:21pm
    Author:  Matt

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player, AudioFormatManager& formatManagerToUse, AudioThumbnailCache& cacheToUse) : player(_player), waveformDisplay(formatManagerToUse, cacheToUse)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);

    // === Registers for events received === //
    // 
    // Button registers
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    // Slider registers
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    // Slider ranges
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 3.0);
    posSlider.setRange(0.0, 1.0);

    startTimer(500);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("DeckGUI", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void DeckGUI::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    double rowH = getHeight() / 8;
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
    speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
    posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
    loadButton.setBounds(0, rowH * 7, getWidth(), rowH);
}

void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        DBG("MainComponent::buttonClicked: They clicked the play button");
        player->start();
    }
    if (button == &stopButton)
    {
        DBG("MainComponent::buttonClicked: They clicked the stop button");
        player->stop();
    }
    if (button == &loadButton)
    {
        DBG("MainComponent::buttonClicked: They clicked the load button");
        auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
        // - launch out of the main thread
        // - note how we use a lambda function which you've probably
        // not seen before. Please do not worry too much about that. 
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
        {
            File chosenFile = chooser.getResult();
            player->loadURL(URL{ chosenFile });
            waveformDisplay.loadURL(URL{ chosenFile });
        });
    }
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == & volSlider)
     {
         DBG("MainComponent::sliderValueChanged: Volume changed to " << volSlider.getValue());
         player->setGain(slider->getValue());
     }
     if (slider == &speedSlider)
     {
         DBG("MainComponent::sliderValueChanged: Speed changed to " << speedSlider.getValue());
         player->setSpeed(slider->getValue());
     }
     if (slider == &posSlider)
     {
         DBG("MainComponent::sliderValueChanged: Position changed to " << posSlider.getValue());
         player->setPositionRelative(slider->getValue());
     }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray& files)
{
    DBG("DeckGUI::isInterestedInFileDrag");
    return true;
}

void DeckGUI::filesDropped(const StringArray& files, int x, int y)
{
    DBG("DeckGUI::filesDropped");
    if (files.size() == 1)
    {
        player->loadURL(URL{ File{ files[0] } });
    }
}

void DeckGUI::timerCallback()
{
    DBG("DeckGUI::timerCallback");
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}