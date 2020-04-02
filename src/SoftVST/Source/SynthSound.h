/*
  ==============================================================================

    SynthSound.h
    Created: 2 Apr 2020 6:16:11pm
    Author:  rgao

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SynthSound : public SynthesiserSound
{
public:
	bool appliesToNote(int midiNoteNumber)
	{
		return true;
	}

	bool appliesToChannel(int midiChannel)
	{
		return true;
	}
};