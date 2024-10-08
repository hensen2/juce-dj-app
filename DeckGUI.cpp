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
    // Draw play button to deck
    addAndMakeVisible(playButton);
    playButton.setImages(true, true, true,
        playImage, 0.7f, Colours::transparentBlack,
        playImage, 1.0f, Colours::transparentBlack,
        playImage, 0.7f, Colours::transparentBlack,
        0.0f);

    // Draw stop button to deck
    addAndMakeVisible(stopButton);
    stopButton.setImages(true, true, true,
        stopImage, 0.7f, Colours::transparentBlack,
        stopImage, 1.0f, Colours::transparentBlack,
        stopImage, 0.7f, Colours::transparentBlack,
        0.0f);

    // Draw load button to deck
    addAndMakeVisible(loadButton);
    loadButton.setImages(true, true, true,
        loadImage, 0.7f, Colours::transparentBlack,
        loadImage, 1.0f, Colours::transparentBlack,
        loadImage, 0.7f, Colours::transparentBlack,
        0.0f);

    // Draw gain slider to deck
    addAndMakeVisible(volSlider);
    volSlider.setRotaryParameters(MathConstants<float>::pi * 1.2f, MathConstants<float>::pi * 2.8f, true);
    addAndMakeVisible(volLabel);
    volLabel.setText("Gain", dontSendNotification);
    volLabel.attachToComponent(&volSlider, true);
    volLabel.setJustificationType(Justification::centred);

    // Draw speed slider to deck
    addAndMakeVisible(speedSlider);
    speedSlider.setRotaryParameters(MathConstants<float>::pi * 1.2f, MathConstants<float>::pi * 2.8f, true);
    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed", dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, true);
    speedLabel.setJustificationType(Justification::centred);

    // Draw position slider to deck
    addAndMakeVisible(posSlider);
    posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(posLabel);
    posLabel.setText("Position", dontSendNotification);
    posLabel.attachToComponent(&posSlider, true);
    posLabel.setJustificationType(Justification::centred);

    // Draw waveform display to deck
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

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);

}

void DeckGUI::resized()
{

    double rowH = getHeight() / 8;
    double paddingH = getHeight() / 16;
    double paddingW = getWidth() / 8;

    // Button bounds
    playButton.setBounds(paddingW, paddingH, 60, 60);
    stopButton.setBounds(getWidth() / 2 - 30, paddingH, 60, 60);
    loadButton.setBounds(getWidth() - 80 - paddingW, paddingH, 80, 60);

    // Rotary Slider bounds
    volSlider.setBounds(paddingW * 1.5, paddingH * 3, getWidth() / 5, getHeight() / 3.5);
    speedSlider.setBounds(getWidth() - (paddingW * 3), paddingH * 3, getWidth() / 5, getHeight() / 3.5);

    // Horizontal Slider bounds
    posSlider.setBounds(paddingW * 1.2, rowH * 4, getWidth() - (paddingW * 2), rowH);

    // Waveform bounds
    waveformDisplay.setBounds(0, rowH * 6, getWidth(), rowH * 2);
    
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
        waveformDisplay.loadURL(URL{ File{ files[0] } });
    }
}

void DeckGUI::timerCallback()
{
    DBG("DeckGUI::timerCallback");
    waveformDisplay.setPositionRelative(player->getPositionRelative());
}

void DeckGUI::loadFile(URL audioURL)
{
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}