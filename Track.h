/*
  ==============================================================================

    Track.h
    Created: 23 Aug 2022 4:29:43pm
    Author:  Matt

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Track
{
public:
    Track(File _file);
    File file;
    URL url;
    String trackTitle;
    String length;
    bool operator==(const String& other) const;
};