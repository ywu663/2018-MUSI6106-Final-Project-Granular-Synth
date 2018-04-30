/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class GranularSynthAudioProcessorEditor  : public AudioProcessorEditor,
                                           public Button::Listener
//                                           public Slider::Listener

{
public:
    GranularSynthAudioProcessorEditor (GranularSynthAudioProcessor&);
    ~GranularSynthAudioProcessorEditor();
    
    void buttonClicked(Button* button) override;
    void openButtonClicked();
//    void sliderValueChanged(Slider* slider) override;

    void paint (Graphics&) override;
    void resized() override;
    


private:
    /** private members declaration */
    GranularSynthAudioProcessor& processor;
    TextButton openButton;
    
//    Slider positionSlider;
//    Label positionLabel;
//    Slider randomPositionSlider;
//    Label randomPositionLabel;
//    Slider densitySlider;
//    Label densityLabel;
//    Slider randomDensitySlider;
//    Label randomDensityLabel;
//    Slider transposeSlider;
//    Label transposeLabel;
//    Slider randomTransposeSlider;
//    Label randomTransposeLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthAudioProcessorEditor)
};
