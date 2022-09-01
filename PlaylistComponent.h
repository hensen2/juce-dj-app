/*
  ==============================================================================

    PlaylistComponent.h
    Created: 19 Jul 2022 1:29:48pm
    Author:  Matt

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>
#include "DeckGUI.h"
#include "Track.h"
#include "DJAudioPlayer.h"


//==============================================================================
/*
*/
class PlaylistComponent : public Component,
    public TableListBoxModel,
    public Button::Listener,
    public TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1,
        DeckGUI* _deckGUI2,
        DJAudioPlayer* _djAudioPlayer);
    ~PlaylistComponent();

    void paint(Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics&,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;
    void paintCell(Graphics&,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;




private:
    std::vector<Track> tracks;
    TextButton loadToDeck1Btn{ "Load in Deck 1" };
    TextButton loadToDeck2Btn{ "Load in Deck 2" };
    TextButton importBtn{ "Import Tracks" };
    TextButton deleteButton{ "Delete" };
    TableListBox playlist;
    TextEditor searchBox;

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    DJAudioPlayer* djAudioPlayer;

    FileChooser fChooser{ "Select a file..." };
    String getLength(URL audioURL);
    String secToMin(int seconds);


    void importToPlaylist();
    void savePlaylist();
    void loadPlaylist();
    void searchPlaylist(String searchText);
    int findTrackTitle(String searchText);
    void deleteFromTracks(int id);
    bool isInTracks(String fileNameWithoutExtension);
    void loadInDeck(DeckGUI* deckGUI);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};