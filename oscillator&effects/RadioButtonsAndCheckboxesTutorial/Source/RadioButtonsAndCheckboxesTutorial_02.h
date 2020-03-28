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

 name:             RadioButtonsAndCheckboxesTutorial
 version:          2.0.0
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
        GenderButtons = 1001
    };

    MainContentComponent()
    {
        addAndMakeVisible (genderLabel);

        addAndMakeVisible (maleButton);
        addAndMakeVisible (femaleButton);
        maleButton  .onClick = [this] { updateToggleState (&maleButton,   "Male");   };
        femaleButton.onClick = [this] { updateToggleState (&femaleButton, "Female"); };

        maleButton  .setRadioGroupId (GenderButtons);
        femaleButton.setRadioGroupId (GenderButtons);

        addAndMakeVisible (hobbiesLabel);

        addAndMakeVisible (sportButton);
        addAndMakeVisible (artButton);
        addAndMakeVisible (filmButton);
        sportButton.onClick = [this] { updateToggleState (&sportButton, "Sport"); };
        artButton  .onClick = [this] { updateToggleState (&artButton,   "Art");   };
        filmButton .onClick = [this] { updateToggleState (&filmButton,  "Film");  };

        maleButton  .setClickingTogglesState (true);
        femaleButton.setClickingTogglesState (true);
        sportButton .setClickingTogglesState (true);
        artButton   .setClickingTogglesState (true);
        filmButton  .setClickingTogglesState (true);

        setSize (400, 300);
    }

    void resized() override
    {
        genderLabel .setBounds (10, 10, getWidth() - 20, 20);
        maleButton  .setBounds (20, 40, getWidth() - 30, 20);
        femaleButton.setBounds (20, 70, getWidth() - 30, 20);

        hobbiesLabel.setBounds (10, 110, getWidth() - 20, 20);
        sportButton .setBounds (20, 140, getWidth() - 30, 20);
        artButton   .setBounds (20, 170, getWidth() - 30, 20);
        filmButton  .setBounds (20, 200, getWidth() - 30, 20);
    }

    void updateToggleState (Button* button, String name)
    {
        auto state = button->getToggleState();
        String stateString = state ? "ON" : "OFF";

        Logger::outputDebugString (name + " Button changed to " + stateString);
    }

private:
    Label genderLabel         { {}, "I identify my gender as..."};
    TextButton maleButton     { "Male" },
               femaleButton   { "Female" };

    Label hobbiesLabel        { {}, "My hobbies are..." };
    TextButton sportButton    { "Sport" },
               artButton      { "Art" },
               filmButton     { "Film"};

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
