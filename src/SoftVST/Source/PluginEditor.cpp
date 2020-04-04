#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoftVstAudioProcessorEditor::SoftVstAudioProcessorEditor (SoftVstAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), envGUI(p), oscGUI(p), filterGUI(p), gainGUI(p)
{
    setSize(800, 600);

	// Add Title
	title.setFont(Font(48.0f, Font::bold));
	title.setColour(Label::textColourId, Colours::fuchsia);
	title.setText("SoftVST", dontSendNotification);
	title.setJustificationType(Justification::centred);
	addAndMakeVisible(title);

	// Add UI Elements
	addAndMakeVisible(oscGUI);
	addAndMakeVisible(envGUI);
	addAndMakeVisible(filterGUI);
	addAndMakeVisible(gainGUI);
}

SoftVstAudioProcessorEditor::~SoftVstAudioProcessorEditor()
{
}

//==============================================================================
void SoftVstAudioProcessorEditor::paint (Graphics& g)
{
	g.fillAll(Colours::lightgoldenrodyellow);
}

void SoftVstAudioProcessorEditor::resized()
{	
	// Title gui placement
	juce::Rectangle<int> area = getLocalBounds();
	int headerHeight = 70;
	title.setBounds(area.removeFromTop(headerHeight));

	// Componenets gui placement 
	Grid grid;
	using Track = Grid::TrackInfo;

	grid.templateRows = { Track(1_fr), Track(2_fr), Track(2_fr) };
	grid.templateColumns = { Track(1_fr), Track(1_fr) };
	grid.items = { nullptr, nullptr, GridItem(oscGUI), GridItem(envGUI), GridItem(filterGUI), GridItem(gainGUI) };
	grid.performLayout(getLocalBounds());
}
