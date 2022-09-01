/*
  ==============================================================================

    Track.cpp
    Created: 23 Aug 2022 4:29:43pm
    Author:  Matt

  ==============================================================================
*/

#include "Track.h"
#include <filesystem>

Track::Track(File _file) : file(_file),
trackTitle(_file.getFileNameWithoutExtension()),
url(URL{ _file })
{
    DBG("Created new track with title: " << trackTitle);
}

bool Track::operator==(const String& other) const
{
    return trackTitle == other;
}