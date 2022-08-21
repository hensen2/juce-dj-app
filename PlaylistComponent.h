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

//==============================================================================
/*
*/
class PlaylistComponent  : public Component,
    public TableListBoxModel,
    public Button::Listener,
    public TextEditor::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (Graphics&) override;
    void resized() override;

    int getNumRows() override;

    void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;

    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;

    //void textEditorTextChanged(TextEditor& editor) override;

    /*
    void deleteFile(); //Deleting files from the playlist by user row number clicked
    void loadFileIntoDeck1(); //Loads playlist files into the deckGUI component, indicating as deck 1
    void loadFileIntoDeck2(); //Loads playlist files into the deckGUI component, indicating as deck 2
    String fileToDeckGUI;
    std::string stdstring;
    */

private:
    TableListBox tableComponent;

    std::vector<std::string> trackTitles;

    //AudioFormatManager formatManager;
    //Array<juce::File> myFiles;
    //std::vector <std::string > existingFiles;
    //double duration;
    //String fileName;
    //Array<double> myFilesDuration;
    //Array<String> fileNames;
    //TextButton libraryLoadButton{ "Load into library" };
    //bool historyClicked = false;
    //int duplicateIndex = -1;
    //TextButton libraryDeleteButton;
    //TextButton libraryDeck1;
    //TextButton libraryDeck2;
    //TextEditor librarySearch{ "Search for tracks..." };
    //String searchInput;

    //double rowclicked;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
