/*
 ==============================================================================
 This file was auto-generated!
 It contains the basic framework code for a JUCE plugin editor.
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
GranularSynthAudioProcessorEditor::GranularSynthAudioProcessorEditor (GranularSynthAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    addAndMakeVisible(openButton);
    openButton.setButtonText("Open...");
    openButton.addListener(this);
    setSize (800, 600);
    
    
    //position
    addAndMakeVisible(positionSlider);
    positionSlider.setRange(0.0001f, 1.0f, 0.01);
    positionSlider.addListener(this);
    addAndMakeVisible(positionLabel);
    positionLabel.setJustificationType(juce::Justification::centred);
    positionLabel.setText("Position", dontSendNotification);
    //random position
    addAndMakeVisible(randomPositionSlider);
    randomPositionSlider.setRange(0.0f, 1.0f, 0.01);
    randomPositionSlider.addListener(this);
    addAndMakeVisible(randomPositionLabel);
    randomPositionLabel.setJustificationType(juce::Justification::centred);
    randomPositionLabel.setText("Random Position", dontSendNotification);
    //duration
    addAndMakeVisible(durationSlider);
    durationSlider.setRange(0.0001f, 4.0f, 0.001);
    durationSlider.addListener(this);
    addAndMakeVisible(durationLabel);
    durationLabel.setJustificationType(juce::Justification::centred);
    durationLabel.setText("Duration", dontSendNotification);
    //random duration
    addAndMakeVisible(randomDurationSlider);
    randomDurationSlider.setRange(0.0f, 1.0f, 0.001);
    randomDurationSlider.addListener(this);
    addAndMakeVisible(randomDurationLabel);
    randomDurationLabel.setJustificationType(juce::Justification::centred);
    randomDurationLabel.setText("Random Duration", dontSendNotification);
    //density
    addAndMakeVisible(densitySlider);
    densitySlider.setRange(0.0001f, 80.0f, 0.001);
    densitySlider.addListener(this);
    addAndMakeVisible(densityLabel);
    densityLabel.setJustificationType(juce::Justification::centred);
    densityLabel.setText("Density", dontSendNotification);
    //random density
    addAndMakeVisible(randomDensitySlider);
    randomDensitySlider.setRange(0.0f, 1.0f, 0.001);
    randomDensitySlider.addListener(this);
    addAndMakeVisible(randomDensityLabel);
    randomDensityLabel.setJustificationType(juce::Justification::centred);
    randomDensityLabel.setText("Random Density", dontSendNotification);
    //transpose
    addAndMakeVisible(transposeSlider);
    transposeSlider.setRange(-48.0f, 48.0f, 0.01);
    transposeSlider.addListener(this);
    addAndMakeVisible(transposeLabel);
    transposeLabel.setJustificationType(juce::Justification::centred);
    transposeLabel.setText("Transposition", dontSendNotification);
    //random transpose
    addAndMakeVisible(randomTransposeSlider);
    randomTransposeSlider.setRange(0.0f, 24.0f, 0.001);
    randomTransposeSlider.addListener(this);
    addAndMakeVisible(randomTransposeLabel);
    randomTransposeLabel.setJustificationType(juce::Justification::centred);
    randomTransposeLabel.setText("Random Trans", dontSendNotification);
    //volume
    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0.0f, 1.0f, 0.001);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeLabel);
    volumeLabel.setJustificationType(juce::Justification::centred);
    volumeLabel.setText("Volume", dontSendNotification);
    
    
    
}

GranularSynthAudioProcessorEditor::~GranularSynthAudioProcessorEditor()
{
}

//==============================================================================
void GranularSynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    g.setColour (Colours::white);
    g.setFont (30.0f);
    g.drawFittedText ("Grains", 30, 30, 200, 20, Justification::centredLeft, 1);
    g.setFont (15.0f);
}

void GranularSynthAudioProcessorEditor::resized()
{
    int linespace = 40;
    const int border = 125;
    openButton.setBounds(getWidth() - 100, 45, 80, 20);
    positionSlider.setBounds(10 + border, 45 + linespace, getWidth() - 20 - border, 20);
    positionLabel.setBounds(10, 45 + linespace, 10 + border, 20);
    randomPositionSlider.setBounds(10 + border, 45 + 2 * linespace, getWidth() - 20 - border, 20);
    randomPositionLabel.setBounds(10, 45 + 2 * linespace, 10 + border, 20);
    durationSlider.setBounds(10 + border, 45 + 3 * linespace, getWidth() - 20 - border, 20);
    durationLabel.setBounds(10, 45 + 3 * linespace, 10 + border, 20);
    randomDurationSlider.setBounds(10 + border, 45 + 4 * linespace, getWidth() - 20 - border, 20);
    randomDurationLabel.setBounds(10, 45 + 4 * linespace, 10 + border, 20);
    densitySlider.setBounds(10 + border, 45 + 5 * linespace, getWidth() - 20 - border, 20);
    densityLabel.setBounds(10, 45 + 5 * linespace, 10 + border, 20);
    randomDensitySlider.setBounds(10 + border, 45 + 6 * linespace, getWidth() - 20 - border, 20);
    randomDensityLabel.setBounds(10, 45 + 6 * linespace, 10 + border, 20);
    transposeSlider.setBounds(10 + border, 45 + 7 * linespace, getWidth() - 20 - border, 20);
    transposeLabel.setBounds(10, 45 + 7 * linespace, 10 + border, 20);
    randomTransposeSlider.setBounds(10 + border, 45 + 8 * linespace, getWidth() - 20 - border, 20);
    randomTransposeLabel.setBounds(10, 45 + 8 * linespace, 10 + border, 20);
    volumeSlider.setBounds(10 + border, 45 + 9 * linespace, getWidth() - 20 - border, 20);
    volumeLabel.setBounds(10, 45 + 9 * linespace, 10 + border, 20);
}

//==============================================================================
/** button stuffs */
void GranularSynthAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &openButton) {
        openButtonClicked();
    }
}

/** slider stuffs */
void GranularSynthAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
//    if (!processor.getBypass())
//    {
//        if (slider == &modWidthSlider)
//        {
//            processor.setVibratoParam(CVibrato::kParamModWidthInS, (float)slider->getValue()/1000);
//        }
//        else if (slider == &modFreqSlider)
//        {
//            processor.setVibratoParam(CVibrato::kParamModFreqInHz, (float)slider->getValue());
//        }
//    }
}

void GranularSynthAudioProcessorEditor::openButtonClicked()
{
    FileChooser chooser ("Select a File to open...", // 1
                         File::nonexistent,
                         "*.wav", "*.aif", "*.aiff");
    
    if(chooser.browseForFileToOpen()){  // 2
        const File file (chooser.getResult());
        String path (file.getFullPathName());
        std::swap(processor.chosenPath, path);
    }
}




//==============================================================================
/** private functions definition */
