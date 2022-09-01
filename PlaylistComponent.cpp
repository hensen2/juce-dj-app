/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 19 Jul 2022 1:29:48pm
    Author:  Matt

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <fstream>
#include "DeckGUI.h"
#include <algorithm>

//==============================================================================
PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1,
                                     DeckGUI* _deckGUI2,
                                     DJAudioPlayer* _djAudioPlayer
                                    ) : deckGUI1(_deckGUI1),
                                        deckGUI2(_deckGUI2),
                                        djAudioPlayer(_djAudioPlayer)

{

    playlist.getHeader().addColumn("Track Title", 1, 400);
    playlist.getHeader().addColumn("Duration", 2, 200);
    playlist.getHeader().addColumn("", 3, 200);
    playlist.setModel(this);

    loadPlaylist();

    addAndMakeVisible(playlist);
    addAndMakeVisible(importBtn);
    addAndMakeVisible(searchBox);
    addAndMakeVisible(loadToDeck1Btn);
    addAndMakeVisible(loadToDeck2Btn);

    importBtn.addListener(this);
    loadToDeck2Btn.addListener(this);
    loadToDeck1Btn.addListener(this);

    searchBox.addListener(this);
    searchBox.setTextToShowWhenEmpty("Type to search tracks...", Colours::orange);
    //textbox onTextChange
    searchBox.onTextChange = [this] { searchPlaylist(searchBox.getText()); };
}

PlaylistComponent::~PlaylistComponent()
{
    savePlaylist();
}

void PlaylistComponent::paint(Graphics& g)
{

    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));   // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);
}

void PlaylistComponent::resized()
{
    double rowH = getHeight() / 8;

    playlist.setBounds(0, rowH, getWidth(), rowH * 6);
    loadToDeck1Btn.setBounds(0, rowH * 7, getWidth() / 2, rowH);
    loadToDeck2Btn.setBounds(getWidth() / 2, rowH * 7, getWidth() / 2, rowH);
    searchBox.setBounds(0, 0, getWidth() / 2, rowH);
    importBtn.setBounds(getWidth() / 2, 0, getWidth() / 2, rowH);

}

int PlaylistComponent::getNumRows()
{
    return tracks.size();
}

void PlaylistComponent::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(Colours::orange);
    }
    else {
        g.fillAll(Colours::darkgrey);
    }

}
void PlaylistComponent::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)
        {
            g.drawText(tracks[rowNumber].trackTitle,
                2,
                0,
                width - 4,
                height,
                Justification::centredLeft,
                true
            );
        }
        if (columnId == 2)
        {
            g.drawText(tracks[rowNumber].length,
                2,
                0,
                width - 4,
                height,
                Justification::centred,
                true
            );
        }
    }

}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Delete" };
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);


            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

//Function to import tracks to playlist
void PlaylistComponent::importToPlaylist()
{
    DBG("PlaylistComponent::importToPlaylist called");

    //initialize file chooser
    auto fileChooserFlags = FileBrowserComponent::canSelectFiles;
    fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
    {
        for (const File& file : chooser.getResults())
        {
            String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!isInTracks(fileNameWithoutExtension)) // if not already loaded
            {
                Track importedTrack{ file };
                URL audioURL{ file };
                importedTrack.length = getLength(audioURL);
                tracks.push_back(importedTrack);
                playlist.updateContent();
                DBG("loaded file: " << importedTrack.trackTitle);
            }
        };
    });
}


String PlaylistComponent::getLength(URL audioURL)
{
    djAudioPlayer->loadURL(audioURL);
    double seconds{ djAudioPlayer->getLengthInSeconds() };
    String minutes{ secToMin(seconds) };
    return minutes;
}

String PlaylistComponent::secToMin(int seconds)
{
    //Minutes in string
    String minStr{ std::to_string(seconds / 60) };
    //Seconds in string
    String secStr{ std::to_string(seconds % 60) };
    //return minutes and seconds in a single string
    return String{ minStr + ":" + secStr };
}
bool PlaylistComponent::isInTracks(String fileNameWithoutExtension)
{
    return (std::find(tracks.begin(), tracks.end(), fileNameWithoutExtension) != tracks.end());
}



void PlaylistComponent::buttonClicked(Button* button)
{
    if (button == &importBtn)
    {
        DBG("Load button was clicked");
        importToPlaylist();
        playlist.updateContent();
    }
    else if (button == &loadToDeck1Btn)
    {
        DBG("Load to deck 1 button was clicked");
        loadInDeck(deckGUI1);
    }
    else if (button == &loadToDeck2Btn)
    {
        DBG("Load to deck 2 button was clicked");
        loadInDeck(deckGUI2);
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());
        DBG(tracks[id].trackTitle + " removed from Library");
        //delete
        deleteFromTracks(id);
        playlist.updateContent();
    }
}
void PlaylistComponent::savePlaylist()
{
    // create .csv to save library
    std::ofstream playlist("playlist.txt");

    // save library to file
    for (Track& t : tracks)
    {
        playlist << t.file.getFullPathName() << "," << t.length << "\n";
    }
}
void PlaylistComponent::loadPlaylist()
{
    //reading from saved playlist.txt
    std::ifstream playlist("playlist.txt");
    std::string filePath;
    std::string length;

    // Read data, line by line
    if (playlist.is_open())
    {
        while (getline(playlist, filePath, ',')) {
            File file{ filePath };
            Track newTrack{ file };

            getline(playlist, length);
            newTrack.length = length;
            tracks.push_back(newTrack);
        }
    }
    playlist.close();
}
void PlaylistComponent::searchPlaylist(String keyword)
{
    DBG("Searching library for: " << keyword);
    if (keyword != "")
    {
        int rowNumber = findTrackTitle(keyword);
        //highlight track matching keyword
        playlist.selectRow(rowNumber);
    }
    else
    {
        playlist.deselectAllRows();
    }
}
int PlaylistComponent::findTrackTitle(String keyword)
{
    // finds index where track title contains the keyword search
    auto it = find_if(tracks.begin(), tracks.end(),
        [&keyword](const Track& obj) {return obj.trackTitle.contains(keyword); });
    int i = -1;

    if (it != tracks.end())
    {
        DBG("Found Track!");
        i = std::distance(tracks.begin(), it);
        DBG(i);
    }

    return i;
}
void PlaylistComponent::deleteFromTracks(int id)
{
    tracks.erase(tracks.begin() + id);
}

//Function to load tracks to deck
void PlaylistComponent::loadInDeck(DeckGUI* deckGUI)
{
    int selectedRow{ playlist.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Adding: " << tracks[selectedRow].trackTitle << " to Deck");
        deckGUI->loadFile(tracks[selectedRow].url);
    }
}