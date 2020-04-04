# SoftVST Report

Team SoftVST: Sparsh Bansal, Richard Gao

_Start Date: Week of Feb 18th_

_End Date: Report due April 4th_

## Project Goal

The goal of our project is to create a VST with a GUI using the JUCE framework as well as get more familiar with C++. Our lower bound would be a program that is an ocsillator with a few adjustable parameters, while a stretch goal is to program additional effects delay and distortion. 

## Learning Goals

Our learning goals are to acquire more familiarity with C and learn C++ by working on a project that interests us: developing software to interface with DAWs. Learning the basics of audio programming is also part of our learning goal, especially understanding the software behind synthesizers and common effects. This may include researching about DSP and signal processing.

## Progress

We have explored different frameworks and libraries used to program audio plugins but have decided to use the JUCE framework. After doing so, we have ran through tutorials to get more familiar with their libraries and started building out the VST with a volume slider in place that takes in midi input. As for resources, we feel that we have a suitable amout of material to learn from and work with, mainly the JUCE tutorial online as well as the book *Designing Audio Effect Plugins in C++ by Will C. Pirkle* and *The Audio Programming Book by Richard Boulanger and Victor Lazzarini*.

## Next Steps

* Finish walking through applicable tutorials (Richard & Sparsh)
  * We are looking into which parts of the JUCE framework that we need to implement our MVP and going through the necessary tutorials.

* Fix AudioPluginHost bug (Sparsh & Richard)
  * Currently the debugging tool for JUCE is not connecting with our VST application. This makes it difficult to test and debug. We are looking into why the tool is not allowing the right connects to surface.

* Explore the oscillator code (Richard & Sparsh)
  * We are going to take a deeper look into the oscillator demo and extract the features that makes sense for our project.


Github: https://github.com/hardlyrichie/SoftVST

Trello: https://trello.com/b/bz26WOs8
