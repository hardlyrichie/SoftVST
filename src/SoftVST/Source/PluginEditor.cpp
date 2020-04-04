#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SoftVstAudioProcessorEditor::SoftVstAudioProcessorEditor (SoftVstAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), envGUI(p), oscGUI(p), filterGUI(p)
{
    setSize(800, 600);

	// Add Title
	title.setFont(Font(48.0f, Font::bold));
	title.setColour(Label::textColourId, Colours::fuchsia);
	title.setText("SoftVST", dontSendNotification);
	title.setJustificationType(Justification::centred);
	addAndMakeVisible(title);

	// Add UI Elements
	gainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	gainSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
	gainSlider.setTextValueSuffix("dB");
	gainSlider.setRange(-60.0f, 0.0f, 0.01f);
	gainSlider.setValue(-20.0f);
	addAndMakeVisible(gainSlider);

	gainLabel.setFont(Font(24.0f, Font::bold));
	gainLabel.setText("Master Fader", dontSendNotification);
	gainLabel.attachToComponent(&gainSlider, false);
	gainLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(gainLabel);

	addAndMakeVisible(oscGUI);
	addAndMakeVisible(envGUI);
	addAndMakeVisible(filterGUI);

	gainSliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,
			GAIN_ID, gainSlider);
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
	grid.items = { nullptr, nullptr, GridItem(oscGUI), GridItem(envGUI), GridItem(filterGUI), GridItem(gainSlider) };

	gainSlider.setSize(10, 10);

	grid.performLayout(getLocalBounds());
}
