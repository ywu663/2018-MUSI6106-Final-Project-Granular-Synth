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
{
public:
    GranularSynthAudioProcessorEditor (GranularSynthAudioProcessor&);
    ~GranularSynthAudioProcessorEditor();
    
    //==============================================================================
    /** button stuffs */
    void buttonClicked(Button* button) override;
    void openButtonClicked();
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    /** private members declaration */
    GranularSynthAudioProcessor& processor;
    TextButton openButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthAudioProcessorEditor)
};
