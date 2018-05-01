/*
 ==============================================================================
 This file was auto-generated!
 It contains the basic framework code for a JUCE plugin editor.
 ==============================================================================
 */

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include <iostream>

using namespace std;


//==============================================================================
/**
 */
class GranularSynthAudioProcessorEditor  : public AudioProcessorEditor,
public Slider::Listener,
public Button::Listener
{
public:
    GranularSynthAudioProcessorEditor (GranularSynthAudioProcessor&);
    ~GranularSynthAudioProcessorEditor();
    
    //==============================================================================
    /** button stuffs */
    void buttonClicked(Button* button) override;
    void openButtonClicked();
    //==============================================================================
    /** sliders */
    void sliderValueChanged(Slider* slider) override;
    
    void paint (Graphics&) override;
    void resized() override;
    

    
private:
    /** private members declaration */
    GranularSynthAudioProcessor& processor;
    TextButton openButton;
    
    
    //position
    Slider positionSlider;
    Label positionLabel;
    Slider randomPositionSlider;
    Label randomPositionLabel;
    //duration
    Slider durationSlider;
    Label durationLabel;
    Slider randomDurationSlider;
    Label randomDurationLabel;
    //density
    Slider densitySlider;
    Label densityLabel;
    Slider randomDensitySlider;
    Label randomDensityLabel;
    //transpose
    Slider transposeSlider;
    Label transposeLabel;
    Slider randomTransposeSlider;
    Label randomTransposeLabel;
    //volume
    Slider volumeSlider;
    Label volumeLabel;
    
    //plan to add more window choice to the grain other than han
    
    //list for sample selection
    
    //waveform
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthAudioProcessorEditor)
};
