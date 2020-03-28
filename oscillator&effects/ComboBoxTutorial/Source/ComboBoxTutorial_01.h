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

 name:             ComboBoxTutorial
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Displays combo boxes.

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
    MainContentComponent()
    {
        addAndMakeVisible (textLabel);
        textLabel.setFont (textFont);

        // add items to the combo-box
        /* Each of these text strings is associated with an ID number(an int value).
        You can query which item is currently selected, either by :

            a. retrieving the currently selected ID using the ComboBox::getSelectedId() 
            function; or
            b. requesting the currently displayed text using the ComboBox::getText() function.
        */
        addAndMakeVisible (styleMenu);
        styleMenu.addItem ("Plain",  1);
        styleMenu.addItem ("Bold",   2);
        styleMenu.addItem ("Italic", 3);

        styleMenu.onChange = [this] { styleMenuChanged(); };
        styleMenu.setSelectedId (1);

        setSize (400, 200);
    }

    void resized() override
    {
        textLabel.setBounds (10, 10, getWidth() - 20, 20);
        styleMenu.setBounds (10, 40, getWidth() - 20, 20);
    }

private:
    /* The styleMenuChanged() function handles changes to the styleMenu object: */
    void styleMenuChanged()
    {
        switch (styleMenu.getSelectedId())
        {
            case 1: textFont.setStyleFlags (Font::plain);  break;
            case 2: textFont.setStyleFlags (Font::bold);   break;
            case 3: textFont.setStyleFlags (Font::italic); break;
            default: break;
        }

        textLabel.setFont (textFont);
    }

    //==============================================================================
    Label textLabel { {}, "The quick brown fox jumps over the lazy dog." };
    Font textFont   { 12.0f };
    ComboBox styleMenu;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};
