/*
  ==============================================================================

    SynthVoice.h
    Created: 2 Apr 2020 6:15:58pm
    Author:  rgao

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public SynthesiserVoice
{
public:
	bool canPlaySound(SynthesiserSound* sound)
	{
		return dynamic_cast<SynthSound*>(sound) != nullptr;
	}

	void startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int
		currentPitchWheelPosition)
	{
		frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		std::cout << midiNoteNumber << std::endl;
		Logger::outputDebugString(std::to_string(midiNoteNumber));
	}

	void stopNote(float velocity, bool allowTailOff)
	{
		clearCurrentNote();
	}

	void pitchWheelMoved(int newPitchWheelValue)
	{

	}

	void controllerMoved(int controllerNumber, int newControllerValue)
	{

	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
	{

	}

private:
	double level;
	double frequency;
};
