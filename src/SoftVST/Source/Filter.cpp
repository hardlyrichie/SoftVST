#include <JuceHeader.h>
#include "Filter.h"

//==============================================================================
Filter::Filter(SoftVstAudioProcessor& p) : processor(p)
{
	setSize(400, 400);

	// Configure combobox settings
	filterMenu.addItem("Low-pass", 1);
	filterMenu.addItem("High-pass", 2);
	filterMenu.addItem("Band-pass", 3);
	filterMenu.setJustificationType(Justification::centred);
	addAndMakeVisible(filterMenu);

	// Configure slider settings
	freqSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	freqSlider.setRange(20.0f, 10000.0f);
	freqSlider.setValue(400.0f);
	freqSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	freqSlider.setTextValueSuffix("Hz");
	// TODO: Fix skewfactor
	freqSlider.setSkewFactorFromMidPoint(1000.0);
	addAndMakeVisible(freqSlider);

	resSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	resSlider.setRange(1.0f, 5.0f);
	resSlider.setValue(1.0f);
	resSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 70, 20);
	addAndMakeVisible(resSlider);
	
	// Filter label settings
	filterLabel.setFont(Font(24.0f, Font::bold));
	filterLabel.setText("Filter", dontSendNotification);
	filterLabel.attachToComponent(&filterMenu, false);
	filterLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(filterLabel);

	freqLabel.setFont(Font(18.0f));
	freqLabel.setText("Freq", dontSendNotification);
	freqLabel.attachToComponent(&freqSlider, false);
	freqLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(freqLabel);

	resLabel.setText("Q", dontSendNotification);
	resLabel.attachToComponent(&resSlider, false);
	resLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(resLabel);

	// Setup apvts attachments
	filterMenuAttachment =
		std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.apvts,
			FILTER_ID, filterMenu);
	freqSliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,
			FREQ_ID, freqSlider);
	resSliderAttachment =
		std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.apvts,
			RES_ID, resSlider);
}

Filter::~Filter()
{
}

void Filter::paint (Graphics& g)
{
	g.fillAll(Colours::mediumvioletred);
}

void Filter::resized()
{
	juce::Rectangle<int> area = getLocalBounds().reduced(100);
	area.setY(area.getY() - 50);
	filterMenu.setBounds(area);

	Grid grid;
	using Track = Grid::TrackInfo;

	grid.templateRows = { Track(2_fr), Track(1_fr) };
	grid.templateColumns = { Track(1_fr), Track(1_fr) };
	grid.items = { nullptr, nullptr, GridItem(freqSlider), GridItem(resSlider) };
	grid.performLayout(getLocalBounds());
}
