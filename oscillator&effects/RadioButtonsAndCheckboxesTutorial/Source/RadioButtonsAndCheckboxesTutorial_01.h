/*
  ==============================================================================

   This file is pSawtooth of the JUCE tutorials.
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

 name:             RadioButtonsAndCheckboxesTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Explores radio buttons and check boxes.

 dependencies:     juce_core, juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics
 exporters:        xcode_mac, vs2017, linux_make, xcode_iphone, androidstudio

 type:             Component
 mainClass:        MainContentComponent

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/


#pragma once

//==============================================================================
class MainContentComponent   : public Component
{
public:
    //==============================================================================
    enum RadioButtonIds
    {
        EffectButtons = 1001
    };

    MainContentComponent()
    {
        addAndMakeVisible (effectLabel);

        addAndMakeVisible (delayButton);
        addAndMakeVisible (distortionButton);
        delayButton  .onClick = [this] { updateToggleState (&delayButton,   "Delay");   };
        distortionButton.onClick = [this] { updateToggleState (&distortionButton, "Distortion"); };

        delayButton  .setRadioGroupId (EffectButtons);
        distortionButton.setRadioGroupId (EffectButtons);

        addAndMakeVisible (waveformLabel);

        addAndMakeVisible (squareButton);
        addAndMakeVisible (sawtoothButton);
        addAndMakeVisible (sineButton);
        squareButton.onClick = [this] { updateToggleState (&squareButton, "Square"); };
        sawtoothButton  .onClick = [this] { updateToggleState (&sawtoothButton,   "Sawtooth");   };
        sineButton .onClick = [this] { updateToggleState (&sineButton,  "Sine");  };

        setSize (400, 300);
    }

    void resized() override
    {
        effectLabel .setBounds (10, 10, getWidth() - 20, 20);
        delayButton  .setBounds (20, 40, getWidth() - 30, 20);
        distortionButton.setBounds (20, 70, getWidth() - 30, 20);

        waveformLabel.setBounds (10, 110, getWidth() - 20, 20);
        squareButton .setBounds (20, 140, getWidth() - 30, 20);
        sawtoothButton   .setBounds (20, 170, getWidth() - 30, 20);
        sineButton  .setBounds (20, 200, getWidth() - 30, 20);
    }

    void updateToggleState (Button* button, String name)
    {
        /* Responding to buttons that can be toggled on or off is similar to responding 
        to regular button clicks.We need to specify the function we want to call when the 
        button is toggled by assigning a lambda function to the Button::onClick helper 
        object(as you can see in the code snippets above).

        The difference is that we also need to check the toggle state of the button in 
        our updateToggleState() function.To do this we can call the Button::getToggleState() 
        function.Again this is a member of the Button class, so this is valid for any 
        button(but again, probably only makes sense if you expect the button to be toggled 
        onand off, as it will return false in other cases).
        */

        auto state = button->getToggleState();
        String stateString = state ? "ON" : "OFF";

        Logger::outputDebugString (name + " Button changed to " + stateString);
    }

private:
    Label effectLabel         { {}, "I want my effect to be..."};
    ToggleButton delayButton   { "Delay" },
                 distortionButton { "Distortion" };

    Label waveformLabel        { {}, "My waveforms are..." };
    ToggleButton squareButton  { "Square" },
                 sawtoothButton    { "Sawtooth" },
                 sineButton   { "Sine"};

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
