#pragma once
#include <JuceHeader.h>
#include "SynthSound.h"
#include "maximilian.h"

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
		env1.trigger = 1;
		level = velocity;

		frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		Logger::outputDebugString(std::to_string(midiNoteNumber));
	}

	void stopNote(float velocity, bool allowTailOff)
	{
		env1.trigger = 0;
		allowTailOff = true;

		if (velocity == 0)
		{
			clearCurrentNote();
		}
	}

	void pitchWheelMoved(int newPitchWheelValue)
	{

	}

	void controllerMoved(int controllerNumber, int newControllerValue)
	{

	}

	void setOscType(float* type)
	{
		wave = *type;
	}

	double getOscType()
	{
		switch (wave)
		{
			case 0:
				return osc1.sinewave(frequency);
			case 1:
				return osc1.saw(frequency);
			case 2:
				return osc1.square(frequency);
			default:
				// TODO: throw exception if none of the waves are selected
				return osc1.sinewave(frequency);
		}
	}

	void setADSR(float* attack)
	{
		//, float* decay, float* sustain, float* release
		env1.setAttack(*attack);
		/*env1.setDecay(*decay);
		env1.setSustain(*sustain);
		env1.setRelease(*release);*/
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
	{
		env1.setDecay(100);
		env1.setSustain(50);
		env1.setRelease(500);

		for (int sample = 0; sample < numSamples; sample++)
		{
			double sound = env1.adsr(getOscType(), env1.trigger) * level;
			double filteredSound = filter1.lores(sound, 1000, 0.1);

			for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
			{
				outputBuffer.addSample(channel, startSample, filteredSound);
			}

			startSample++;
		}
	}

private:
	double level;
	double frequency;
	int wave;
	
	maxiOsc osc1;
	maxiEnv env1;
	maxiFilter filter1;
};
