/*
  ==============================================================================

   This file is part of the JUCE tutorials.
   Copyright (c) 2017 - ROLI Ltd.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             DSPDelayLineTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Explores the DSP module.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_plugin_client, juce_audio_processors,
                   juce_audio_utils, juce_core, juce_data_structures, juce_dsp,
                   juce_events, juce_graphics, juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2017, linux_make

 type:             AudioProcessor
 mainClass:        DSPTutorialAudioProcessor

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

// Tutorial link: https://docs.juce.com/master/tutorial_dsp_delay_line.html
#pragma once

//==============================================================================
template <typename Type>
class CustomOscillator
{
public:
    //==============================================================================
    CustomOscillator()
    {
        setWaveform (Waveform::sine);

        auto& gain = processorChain.template get<gainIndex>();
        gain.setRampDurationSeconds (3e-2);
        gain.setGainLinear (Type (0));
    }

    //==============================================================================
    enum class Waveform
    {
        sine,
        saw
    };

    void setWaveform (Waveform waveform)
    {
        switch (waveform)
        {
        case Waveform::sine:
            processorChain.template get<oscIndex>().initialise ([] (Type x)
                                                                {
                                                                    return std::sin (x);
                                                                }, 128);
            break;

        case Waveform::saw:
            processorChain.template get<oscIndex>().initialise ([] (Type x)
                                                                {
                                                                    return jmap (x, Type (-double_Pi), Type (double_Pi), Type (-1), Type (1));
                                                                }, 128);
            break;

        default:
            jassertfalse;
            break;
        }
    }

    //==============================================================================
    void setFrequency (Type newValue, bool force = false)
    {
        processorChain.template get<oscIndex>().setFrequency (newValue, force);
    }

    void setLevel (Type newValue)
    {
        processorChain.template get<gainIndex>().setGainLinear (newValue);
    }

    void reset() noexcept
    {
        processorChain.reset();
    }

    //==============================================================================
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        auto&& outBlock = context.getOutputBlock();
        auto blockToUse = tempBlock.getSubBlock (0, outBlock.getNumSamples());
        juce::dsp::ProcessContextReplacing<float> tempContext (blockToUse);
        processorChain.process (tempContext);

        outBlock.copyFrom (context.getInputBlock()).add (blockToUse);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
        processorChain.prepare (spec);
    }

private:
    //==============================================================================
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;

    enum
    {
        oscIndex,
        gainIndex,
    };

    juce::dsp::ProcessorChain<juce::dsp::Oscillator<Type>, juce::dsp::Gain<Type>> processorChain;
};

//==============================================================================
template <typename Type>
class CabSimulator
{
public:
    //==============================================================================
    CabSimulator()
    {
        auto dir = File::getCurrentWorkingDirectory();

        int numTries = 0;

        while (! dir.getChildFile ("Resources").exists() && numTries++ < 15)
            dir = dir.getParentDirectory();

        auto& convolution = processorChain.template get<convolutionIndex>();
        convolution.loadImpulseResponse (dir.getChildFile ("Resources").getChildFile ("guitar_amp.wav"), true, false, 1024);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        processorChain.prepare (spec);
    }

    //==============================================================================
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        processorChain.process (context);
    }

    //==============================================================================
    void reset() noexcept
    {
        processorChain.reset();
    }

private:
    //==============================================================================
    enum
    {
        convolutionIndex
    };

    juce::dsp::ProcessorChain<juce::dsp::Convolution> processorChain;
};

//==============================================================================
template <typename Type>
class DelayLine
{
public:
    void clear() noexcept
    {
        std::fill (rawData.begin(), rawData.end(), Type (0));
    }

    size_t size() const noexcept
    {
        return rawData.size();
    }

    void resize (size_t newValue)
    {
        rawData.resize (newValue);
        leastRecentIndex = 0;
    }

    Type back() const noexcept
    {
        return rawData[leastRecentIndex];
    }

    // Return the sample located at an offset specified by the function argument 
    // while making sure that the index wraps around the vector [3]. Notice here
    // that we make sure the delay does not exceed the size of the buffer.

    Type get(size_t delayInSamples) const noexcept
    {
        jassert(delayInSamples >= 0 && delayInSamples < size());
        return rawData[(leastRecentIndex + 1 + delayInSamples) % size()];     // [3]
    }

    /** Set the specified sample in the delay line */
    // assigning the sample at an offset specified by the function argument while 
    // making sure that the index wraps around the vector [4]. Here again, we make 
    // sure the delay does not exceed the size of the buffer.

    void set(size_t delayInSamples, Type newValue) noexcept
    {
        jassert(delayInSamples >= 0 && delayInSamples < size());
        rawData[(leastRecentIndex + 1 + delayInSamples) % size()] = newValue; // [4]
    }

    /** Adds a new value to the delay line, overwriting the least recently added sample */
    // Here, we update the least recent index variable by wrapping the index by the size 
    // of the circular buffer

    // Info on a circular buffer: A circular buffer can be essentially implemented as an 
    // array where the index wraps around itself in order to create a circular data structure 
    // that matches the size of the sample buffer block. This allows us to store all the 
    // samples included in the previous block to be accessed in the current block, only 
    // to be overwritten by the current sample block for the next iteration.
    void push(Type valueToAdd) noexcept
    {
        rawData[leastRecentIndex] = valueToAdd; // [1]
        leastRecentIndex = leastRecentIndex == 0 ? size() - 1 : leastRecentIndex - 1; // [2]
    }

private:
    std::vector<Type> rawData;
    size_t leastRecentIndex = 0;
};

//==============================================================================
template <typename Type, size_t maxNumChannels = 2>
class Delay
{
public:
    //==============================================================================
    // Here we defined the behaviour of our delay effect and these include delay times
    // for individual channels, the maximum delay time allowed, the dry/wet level of the 
    // effect and the amount of feedback.

    Delay()
    {
        setMaxDelayTime(2.0f);
        setDelayTime(0, 0.7f);
        setDelayTime(1, 0.5f);
        setWetLevel(0.8f);
        setFeedback(0.5f);
    }

    //==============================================================================
    // Make sure that the size of the delay lines [7] and the delay times in samples
    // [8] are still correct in case the sample rate was changed between sample blocks
    // and initialise the filters with a low-pass filter for now [9]
    void prepare(const juce::dsp::ProcessSpec& spec)
    {
        jassert(spec.numChannels <= maxNumChannels);
        sampleRate = (Type)spec.sampleRate;
        updateDelayLineSize(); // [7]
        updateDelayTime();     // [8]

        // Changed this filter to a high pass filter
        filterCoefs = juce::dsp::IIR::Coefficients<Type>::makeFirstOrderHighPass(sampleRate, Type(1e3));

        for (auto& f : filters)
        {
            f.prepare(spec);
            f.coefficients = filterCoefs;
        }
    }

    //==============================================================================
    void reset() noexcept
    {
        for (auto& f : filters)
            f.reset();     // [5]

        for (auto& dline : delayLines)
            dline.clear(); // [6]
    }

    //==============================================================================
    size_t getNumChannels() const noexcept
    {
        return delayLines.size();
    }

    //==============================================================================
    void setMaxDelayTime(Type newValue)
    {
        jassert(newValue > Type(0));
        maxDelayTime = newValue;
        updateDelayLineSize();         // [1]
    }

    //==============================================================================
    void setFeedback (Type newValue) noexcept
    {
        jassert (newValue >= Type (0) && newValue <= Type (1));
        feedback = newValue;
    }

    //==============================================================================
    void setWetLevel (Type newValue) noexcept
    {
        jassert (newValue >= Type (0) && newValue <= Type (1));
        wetLevel = newValue;
    }

    //==============================================================================
    void setDelayTime(size_t channel, Type newValue)
    {
        if (channel >= getNumChannels())
        {
            jassertfalse;
            return;
        }

        jassert(newValue >= Type(0));
        delayTimes[channel] = newValue;

        updateDelayTime();              // [3]
    }

    //==============================================================================

    // [10]: First, for each sample in the buffer block of each channel, retrieve the 
    // delayed sample from the corresponding delay line.
    // [11]: Then get the current sample from the input block.
    // [12] : Next, calculate the sample to be pushed into the delay line by mixing 
    // the input sample with the delay line output weighted with the feedback parameter
    // using std::tanh().The hyperbolic tangent function allows us to smoothly combine 
    // the two signals without clipping the summed sampleand provides a natural decay.
    // [13] : We then push the sample calculated in the previous step into the delay line.
    // [14] : Finally, calculate the output sample by mixing the input sample with the 
    // delay line output weighted with the dry / wet parameter.
    // [15] : Then assign the sample to the output block.
    // [16] : Filtering the delay line for a more natural sound

    template <typename ProcessContext>
    void process(const ProcessContext& context) noexcept
    {
        auto& inputBlock = context.getInputBlock();
        auto& outputBlock = context.getOutputBlock();
        auto numSamples = outputBlock.getNumSamples();
        auto numChannels = outputBlock.getNumChannels();

        jassert(inputBlock.getNumSamples() == numSamples);
        jassert(inputBlock.getNumChannels() == numChannels);

        for (size_t ch = 0; ch < numChannels; ++ch)
        {
            auto* input = inputBlock.getChannelPointer(ch);
            auto* output = outputBlock.getChannelPointer(ch);
            auto& dline = delayLines[ch];
            auto delayTime = delayTimesSample[ch];
            auto& filter = filters[ch];

            for (size_t i = 0; i < numSamples; ++i)
            {
                auto delayedSample = filter.processSample(dline.get(delayTime));            // [16]
                // auto delayedSample = dline.get(delayTime);                                 // [10] - Commented out to incorporate the high pass filter
                auto inputSample = input[i];                                                // [11]
                auto dlineInputSample = std::tanh(inputSample + feedback * delayedSample); // [12]
                dline.push(dlineInputSample);                                              // [13]
                auto outputSample = inputSample + wetLevel * delayedSample;                 // [14]
                output[i] = outputSample;                                                   // [15]
            }
        }
    }

private:
    //==============================================================================
    std::array<DelayLine<Type>, maxNumChannels> delayLines;
    std::array<size_t, maxNumChannels> delayTimesSample;
    std::array<Type, maxNumChannels> delayTimes;
    Type feedback { Type (0) };
    Type wetLevel { Type (0) };

    std::array<juce::dsp::IIR::Filter<Type>, maxNumChannels> filters;
    typename juce::dsp::IIR::Coefficients<Type>::Ptr filterCoefs;

    Type sampleRate   { Type (44.1e3) };
    Type maxDelayTime { Type (2) };

    //==============================================================================
    // Complete the following function which ensures that the circular buffers of all
    // the delay lines are large enough to accomodate any delay time up to the max 
    // delay time by resizing the vectors [2]

    void updateDelayLineSize()
    {
        auto delayLineSizeSamples = (size_t)std::ceil(maxDelayTime * sampleRate);

        for (auto& dline : delayLines)
            dline.resize(delayLineSizeSamples); // [2]
    }

    //==============================================================================
    // Recalculates the delay times in samples for all the channels based on the new
    // parameter change [4]
    void updateDelayTime() noexcept
    {
        for (size_t ch = 0; ch < maxNumChannels; ++ch)
            delayTimesSample[ch] = (size_t)juce::roundToInt(delayTimes[ch] * sampleRate); // [4]
    }
};

//==============================================================================
template <typename Type>
class Distortion
{
public:
    //==============================================================================
    Distortion()
    {
        auto& waveshaper = processorChain.template get<waveshaperIndex>();
        waveshaper.functionToUse = [] (Type x)
                                   {
                                       return std::tanh (x);
                                   };

        auto& preGain = processorChain.template get<preGainIndex>();
        preGain.setGainDecibels (30.0f);

        auto& postGain = processorChain.template get<postGainIndex>();
        postGain.setGainDecibels (0.0f);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        auto& filter = processorChain.template get<filterIndex>();
        filter.state = FilterCoefs::makeFirstOrderHighPass (spec.sampleRate, 1000.0f);

        processorChain.prepare (spec);
    }

    //==============================================================================
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        processorChain.process (context);
    }

    //==============================================================================
    void reset() noexcept
    {
        processorChain.reset();
    }

private:
    //==============================================================================
    enum
    {
        filterIndex,
        preGainIndex,
        waveshaperIndex,
        postGainIndex
    };

    using Filter = juce::dsp::IIR::Filter<Type>;
    using FilterCoefs = juce::dsp::IIR::Coefficients<Type>;

    juce::dsp::ProcessorChain<juce::dsp::ProcessorDuplicator<Filter, FilterCoefs>,
                              juce::dsp::Gain<Type>, juce::dsp::WaveShaper<Type>, juce::dsp::Gain<Type>> processorChain;
};

//==============================================================================
template <typename Type>
class WaveguideString
{
public:
    //==============================================================================
    WaveguideString()
    {
        setTriggerPosition (Type (0.2));
        setPickupPosition (Type (0.8));
        setDecayTime (Type (0.5));
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRateHz = (Type) spec.sampleRate;
        tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
        filter.prepare (spec);
        updateParameters();
    }

    //==============================================================================
    void reset() noexcept
    {
        forwardDelayLine .clear();
        backwardDelayLine.clear();
    }

    //==============================================================================
    void setFrequency (Type newValueHz)
    {
        freqHz = newValueHz;
        updateParameters();
    }

    //==============================================================================
    void setPickupPosition (Type newValue)
    {
        jassert (newValue >= Type (0) && newValue <= Type (1));
        pickupPos = newValue;
        updateParameters();
    }

    //==============================================================================
    void setTriggerPosition (Type newValue)
    {
        jassert (newValue >= Type (0) && newValue <= Type (1));
        triggerPos = newValue;
        updateParameters();
    }

    //==============================================================================
    void setDecayTime (Type newValue) noexcept
    {
        jassert (newValue >= Type (0) && newValue <= Type (1));
        decayTime = newValue;
        updateParameters();
    }

    //==============================================================================
    void trigger (Type velocity) noexcept
    {
        jassert (velocity >= Type (0) && velocity <= Type (1));
        ignoreUnused (velocity);
    }

    //==============================================================================
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        auto&& outBlock = context.getOutputBlock();
        auto numSamples = outBlock.getNumSamples();
        auto* dst = tempBlock.getChannelPointer (0);
        ignoreUnused (outBlock, numSamples, dst);
    }

private:
    //==============================================================================
    DelayLine<Type> forwardDelayLine;
    DelayLine<Type> backwardDelayLine;
    juce::dsp::IIR::Filter<Type> filter;

    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;

    size_t forwardPickupIndex  { 0 };
    size_t backwardPickupIndex { 0 };
    size_t forwardTriggerIndex { 0 };
    Type decayCoef;

    Type sampleRateHz { Type (1e3) };
    Type freqHz       { Type (1) };
    Type pickupPos    { Type (0) };
    Type triggerPos   { Type (0) };
    Type decayTime    { Type (0) };

    //==============================================================================
    size_t getDelayLineLength() const noexcept
    {
        return forwardDelayLine.size();
    }

    //==============================================================================
    Type processSample() noexcept
    {
        return Type (0);
    }

    //==============================================================================
    void updateParameters()
    {
        reset();
    }
};

//==============================================================================
class Voice  : public juce::MPESynthesiserVoice
{
public:
    Voice()
    {
        auto waveform = CustomOscillator<float>::Waveform::saw;
        processorChain.get<oscIndex>().setWaveform (waveform);

        auto& masterGain = processorChain.get<masterGainIndex>();
        masterGain.setGainLinear (0.7f);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        tempBlock = juce::dsp::AudioBlock<float> (heapBlock, spec.numChannels, spec.maximumBlockSize);
        processorChain.prepare (spec);
    }

    //==============================================================================
    void noteStarted() override
    {
        auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
        auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();

        processorChain.get<oscIndex>().setFrequency (freqHz, true);

        processorChain.get<oscIndex>().setLevel (velocity);
    }

    //==============================================================================
    void notePitchbendChanged () override
    {
        auto freqHz = (float) getCurrentlyPlayingNote().getFrequencyInHertz();
        processorChain.get<oscIndex>().setFrequency (freqHz);
    }

    //==============================================================================
    void noteStopped (bool) override
    {
        processorChain.get<oscIndex>().setLevel (0.0f);
    }

    //==============================================================================
    void notePressureChanged() override {}
    void noteTimbreChanged() override   {}
    void noteKeyStateChanged() override {}

    //==============================================================================
    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        auto block = tempBlock.getSubBlock (0, (size_t) numSamples);
        block.clear();
        juce::dsp::ProcessContextReplacing<float> context (block);
        processorChain.process (context);

        // silence detector
        bool active = false;
        for (size_t ch = 0; ch < block.getNumChannels(); ++ch)
        {
            auto* channelPtr = block.getChannelPointer (ch);

            for (int i = 0; i < numSamples; ++i)
            {
                if (channelPtr[i] != 0.0f)
                {
                    active = true;
                    break;
                }
            }
        }

        if (active)
        {
            juce::dsp::AudioBlock<float> (outputBuffer)
                .getSubBlock ((size_t) startSample, (size_t) numSamples)
                .add (tempBlock);
        }
        else
        {
            clearCurrentNote();
        }
    }

private:
    //==============================================================================
    juce::HeapBlock<char> heapBlock;
    juce::dsp::AudioBlock<float> tempBlock;

    enum
    {
        oscIndex,
        masterGainIndex
    };

    juce::dsp::ProcessorChain<CustomOscillator<float>, juce::dsp::Gain<float>> processorChain;
};

//==============================================================================
class AudioEngine  : public juce::MPESynthesiser
{
public:
    static constexpr size_t maxNumVoices = 4;

    //==============================================================================
    AudioEngine()
    {
        for (size_t i = 0; i < maxNumVoices; ++i)
            addVoice (new Voice);

        setVoiceStealingEnabled (true);
    }

    //==============================================================================
    void prepare (const juce::dsp::ProcessSpec& spec) noexcept
    {
        setCurrentPlaybackSampleRate (spec.sampleRate);

        for (auto* v : voices)
            dynamic_cast<Voice*> (v)->prepare (spec);

        fxChain.prepare (spec);
    }

private:
    //==============================================================================
    enum
    {
        distortionIndex,
        cabSimulatorIndex,
        delayIndex,
        reverbIndex
    };

    juce::dsp::ProcessorChain<Distortion<float>, CabSimulator<float>,
                              Delay<float>, juce::dsp::Reverb> fxChain;

    //==============================================================================
    void renderNextSubBlock (AudioBuffer<float>& outputAudio, int startSample, int numSamples) override
    {
        MPESynthesiser::renderNextSubBlock (outputAudio, startSample, numSamples);

        auto block = juce::dsp::AudioBlock<float> (outputAudio).getSubBlock ((size_t) startSample, (size_t) numSamples);
        auto context = juce::dsp::ProcessContextReplacing<float> (block);
        fxChain.process (context);
    }
};

//==============================================================================
template <typename SampleType>
class AudioBufferQueue
{
public:
    //==============================================================================
    static constexpr size_t order = 9;
    static constexpr size_t bufferSize = 1U << order;
    static constexpr size_t numBuffers = 5;

    //==============================================================================
    void push (const SampleType* dataToPush, size_t numSamples)
    {
        jassert (numSamples <= bufferSize);

        int start1, size1, start2, size2;
        abstractFifo.prepareToWrite (1, start1, size1, start2, size2);

        jassert (size1 <= 1);
        jassert (size2 == 0);

        if (size1 > 0)
            FloatVectorOperations::copy (buffers[(size_t) start1].data(), dataToPush, (int) jmin (bufferSize, numSamples));

        abstractFifo.finishedWrite (size1);
    }

    //==============================================================================
    void pop (SampleType* outputBuffer)
    {
        int start1, size1, start2, size2;
        abstractFifo.prepareToRead (1, start1, size1, start2, size2);

        jassert (size1 <= 1);
        jassert (size2 == 0);

        if (size1 > 0)
            FloatVectorOperations::copy (outputBuffer, buffers[(size_t) start1].data(), (int) bufferSize);

        abstractFifo.finishedRead (size1);
    }

private:
    //==============================================================================
    AbstractFifo abstractFifo { numBuffers };
    std::array<std::array<SampleType, bufferSize>, numBuffers> buffers;
};

//==============================================================================
template <typename SampleType>
class ScopeDataCollector
{
public:
    //==============================================================================
    ScopeDataCollector (AudioBufferQueue<SampleType>& queueToUse)
        : audioBufferQueue (queueToUse)
    {}

    //==============================================================================
    void process (const SampleType* data, size_t numSamples)
    {
        size_t index = 0;

        if (state == State::waitingForTrigger)
        {
            while (index++ < numSamples)
            {
                auto currentSample = *data++;

                if (currentSample >= triggerLevel && prevSample < triggerLevel)
                {
                    numCollected = 0;
                    state = State::collecting;
                    break;
                }

                prevSample = currentSample;
            }
        }

        if (state == State::collecting)
        {
            while (index++ < numSamples)
            {
                buffer[numCollected++] = *data++;

                if (numCollected == buffer.size())
                {
                    audioBufferQueue.push (buffer.data(), buffer.size());
                    state = State::waitingForTrigger;
                    prevSample = SampleType (100);
                    break;
                }
            }
        }
    }

private:
    //==============================================================================
    AudioBufferQueue<SampleType>& audioBufferQueue;
    std::array<SampleType, AudioBufferQueue<SampleType>::bufferSize> buffer;
    size_t numCollected;
    SampleType prevSample = SampleType (100);

    static constexpr auto triggerLevel = SampleType (0.05);

    enum class State { waitingForTrigger, collecting } state { State::waitingForTrigger };
};

//==============================================================================
template <typename SampleType>
class ScopeComponent  : public juce::Component,
                        private Timer
{
public:
    using Queue = AudioBufferQueue<SampleType>;

    //==============================================================================
    ScopeComponent (Queue& queueToUse)
        : audioBufferQueue (queueToUse)
    {
        sampleData.fill (SampleType (0));
        setFramesPerSecond (30);
    }

    //==============================================================================
    void setFramesPerSecond (int framesPerSecond)
    {
        jassert (framesPerSecond > 0 && framesPerSecond < 1000);
        startTimerHz (framesPerSecond);
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        g.fillAll (juce::Colours::black);
        g.setColour (juce::Colours::white);

        auto area = getLocalBounds();
        auto h = (SampleType) area.getHeight();
        auto w = (SampleType) area.getWidth();

        // Oscilloscope
        auto scopeRect = Rectangle<SampleType> { SampleType (0), SampleType (0), w, h / 2 };
        plot (sampleData.data(), sampleData.size(), g, scopeRect, SampleType (1), h / 4);

        // Spectrum
        auto spectrumRect = Rectangle<SampleType> { SampleType (0), h / 2, w, h / 2 };
        plot (spectrumData.data(), spectrumData.size() / 4, g, spectrumRect);
    }

    //==============================================================================
    void resized() override {}

private:
    //==============================================================================
    Queue& audioBufferQueue;
    std::array<SampleType, Queue::bufferSize> sampleData;

    juce::dsp::FFT fft { Queue::order };
    using WindowFun = juce::dsp::WindowingFunction<SampleType>;
    WindowFun windowFun { (size_t) fft.getSize(), WindowFun::hann };
    std::array<SampleType, 2 * Queue::bufferSize> spectrumData;

    //==============================================================================
    void timerCallback() override
    {
        audioBufferQueue.pop (sampleData.data());
        FloatVectorOperations::copy (spectrumData.data(), sampleData.data(), (int) sampleData.size());

        auto fftSize = (size_t) fft.getSize();

        jassert (spectrumData.size() == 2 * fftSize);
        windowFun.multiplyWithWindowingTable (spectrumData.data(), fftSize);
        fft.performFrequencyOnlyForwardTransform (spectrumData.data());

        static constexpr auto mindB = SampleType (-160);
        static constexpr auto maxdB = SampleType (0);

        for (auto& s : spectrumData)
            s = jmap (jlimit (mindB, maxdB, juce::Decibels::gainToDecibels (s) - juce::Decibels::gainToDecibels (SampleType (fftSize))), mindB, maxdB, SampleType (0), SampleType (1));

        repaint();
    }

    //==============================================================================
    static void plot (const SampleType* data,
                      size_t numSamples,
                      Graphics& g,
                      juce::Rectangle<SampleType> rect,
                      SampleType scaler = SampleType (1),
                      SampleType offset = SampleType (0))
    {
        auto w = rect.getWidth();
        auto h = rect.getHeight();
        auto right = rect.getRight();

        auto center = rect.getBottom() - offset;
        auto gain = h * scaler;

        for (size_t i = 1; i < numSamples; ++i)
            g.drawLine ({ jmap (SampleType (i - 1), SampleType (0), SampleType (numSamples - 1), SampleType (right - w), SampleType (right)),
                          center - gain * data[i - 1],
                          jmap (SampleType (i), SampleType (0), SampleType (numSamples - 1), SampleType (right - w), SampleType (right)),
                          center - gain * data[i] });
    }
};

//==============================================================================
class DSPTutorialAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DSPTutorialAudioProcessor()
         : AudioProcessor (BusesProperties().withOutput ("Output", AudioChannelSet::stereo(), true))
    {}

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override
    {
        audioEngine.prepare ({ sampleRate, (uint32) samplesPerBlock, 2 });
        midiMessageCollector.reset (sampleRate);
    }

    void releaseResources() override {}

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        // This is the place where you check if the layout is supported.
        // In this template code we only support mono or stereo.
        if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
         && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
            return false;

        return true;
    }

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override
    {
        ScopedNoDenormals noDenormals;
        auto totalNumInputChannels  = getTotalNumInputChannels();
        auto totalNumOutputChannels = getTotalNumOutputChannels();

        midiMessageCollector.removeNextBlockOfMessages (midiMessages, buffer.getNumSamples());

        for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());

        audioEngine.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
        scopeDataCollector.process (buffer.getReadPointer (0), (size_t) buffer.getNumSamples());
    }

    //==============================================================================
    AudioProcessorEditor* createEditor() override
    {
        return new DSPTutorialAudioProcessorEditor (*this);
    }

    bool hasEditor() const override                                  { return true; }

    //==============================================================================
    const String getName() const override                            { return JucePlugin_Name; }
    bool acceptsMidi() const override                                { return true; }
    bool producesMidi() const override                               { return false; }
    bool isMidiEffect() const override                               { return false; }
    double getTailLengthSeconds() const override                     { return 0.0; }

    //==============================================================================
    int getNumPrograms() override                                    { return 1; }
    int getCurrentProgram() override                                 { return 0; }
    void setCurrentProgram (int) override                            {}
    const String getProgramName (int) override                       { return {}; }
    void changeProgramName (int, const String&) override             {}

    //==============================================================================
    void getStateInformation (MemoryBlock&) override                 {}
    void setStateInformation (const void*, int) override             {}

    //==============================================================================
    MidiMessageCollector& getMidiMessageCollector() noexcept { return midiMessageCollector; }
    AudioBufferQueue<float>& getAudioBufferQueue() noexcept  { return audioBufferQueue; }

private:
    //==============================================================================
    class DSPTutorialAudioProcessorEditor  : public AudioProcessorEditor
    {
    public:
        DSPTutorialAudioProcessorEditor (DSPTutorialAudioProcessor& p)
            : AudioProcessorEditor (&p),
              dspProcessor (p),
              scopeComponent (dspProcessor.getAudioBufferQueue())
        {
            addAndMakeVisible (midiKeyboardComponent);
            addAndMakeVisible (scopeComponent);

            setSize (400, 300);

            auto area = getLocalBounds();
            scopeComponent.setTopLeftPosition (0, 80);
            scopeComponent.setSize (area.getWidth(), area.getHeight() - 100);

            midiKeyboardComponent.setMidiChannel (2);
            midiKeyboardState.addListener (&dspProcessor.getMidiMessageCollector());
        }

        ~DSPTutorialAudioProcessorEditor() override
        {
            midiKeyboardState.removeListener (&dspProcessor.getMidiMessageCollector());
        }

        //==============================================================================
        void paint (Graphics& g) override
        {
            g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        }

        void resized() override
        {
            auto area = getLocalBounds();
            midiKeyboardComponent.setBounds (area.removeFromTop (80).reduced (8));
        }

    private:
        //==============================================================================
        DSPTutorialAudioProcessor& dspProcessor;
        MidiKeyboardState midiKeyboardState;
        MidiKeyboardComponent midiKeyboardComponent { midiKeyboardState, MidiKeyboardComponent::horizontalKeyboard };
        ScopeComponent<float> scopeComponent;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSPTutorialAudioProcessorEditor)
    };

    //==============================================================================
    AudioEngine audioEngine;
    MidiMessageCollector midiMessageCollector;
    AudioBufferQueue<float> audioBufferQueue;
    ScopeDataCollector<float> scopeDataCollector { audioBufferQueue };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSPTutorialAudioProcessor)
};
