/*
  ==============================================================================

    Gain.h
    Created: 3 Apr 2020 7:10:58pm
    Author:  rgao

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Gain    : public Component
{
public:
    Gain();
    ~Gain();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Gain)
};
