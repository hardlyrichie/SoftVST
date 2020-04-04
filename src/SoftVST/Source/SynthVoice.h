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
		env.trigger = 1;
		level = velocity;

		frequency = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
		Logger::outputDebugString(std::to_string(midiNoteNumber));
	}

	void stopNote(float velocity, bool allowTailOff)
	{
		env.trigger = 0;
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

	void setADSR(float* attack, float* decay, float* sustain, float* release)
	{
		env.setAttack(*attack);
		env.setDecay(*decay);
		env.setSustain(*sustain);
		env.setRelease(*release);
	}

	void setFilter(float* type, float* freq, float* res)
	{
		filterType = *type;
		this->freq = *freq;
		this->res = *res;
	}

	void setGain(float* gain)
	{
		this->gain = *gain;
	}

	double getSound()
	{
		double sound = env.adsr(getOscType(), env.trigger) * level;
		switch (filterType)
		{
			case 0:
				return filter.lores(sound, freq, res);
			case 1:
				return filter.hires(sound, freq, res);
			case 2:
				return filter.bandpass(sound, freq, res);
			default:
				return filter.lores(sound, freq, res);
		}
	}

	void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
	{
		for (int sample = 0; sample < numSamples; sample++)
		{
			for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
			{
				outputBuffer.addSample(channel, startSample, getSound() * Decibels::decibelsToGain(gain));
			}

			startSample++;
		}
	}

private:
	double level;
	double frequency;
	int wave;
	int filterType;
	double freq;
	double res;
	double gain;
	
	maxiOsc osc1;
	maxiEnv env;
	maxiFilter filter;
};
