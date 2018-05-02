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


#define pos_ID "pos"
#define pos_NAME "position"
#define randPos_ID "randPos"
#define randPos_NAME "rand pos"
#define dur_ID "dur"
#define dur_NAME "duration"
#define randDur_ID "randDur"
#define randDur_NAME "rand dur"
#define den_ID "den"
#define den_NAME "density"
#define randDen_ID "randDen"
#define randDen_NAME "rand den"
#define trans_ID "trans"
#define trans_NAME "transpose"
#define randTrans_ID "randTrans"
#define randTrans_NAME "rand trans"
#define vol_ID "vol"
#define vol_NAME "volume"
#define reverb_ID "reverb"
#define reverb_NAME "reverb"

//==============================================================================
/**
 */
class GranularSynthAudioProcessorEditor  : public AudioProcessorEditor,
public Slider::Listener,
public Button::Listener,
public Timer,
public ChangeListener
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
    /** AudioThumbnail */
    void thumbnailChanged();
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void timerCallback() override;
    void paint (Graphics&) override;
    void paintIfNoFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds);
    void paintIfFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds);
    void paintPlayHead (Graphics& g, const Rectangle<int>& thumbnailBounds);
    void resized() override;
    

    
    //automation
    AudioProcessorValueTreeState::SliderAttachment* sliderAttachPosition;
    AudioProcessorValueTreeState::SliderAttachment* sliderAttachRandomPosition;
    AudioProcessorValueTreeState::SliderAttachment* sliderAttachDuration;
    AudioProcessorValueTreeState::SliderAttachment* sliderAttachRandomDuration;
    AudioProcessorValueTreeState::SliderAttachment* sliderAttachDensity;
    AudioProcessorValueTreeState::SliderAttachment* sliderAttachRandomDensity;
    AudioProcessorValueTreeState::SliderAttachment* sliderAttachTranspose;
    AudioProcessorValueTreeState::SliderAttachment* sliderAttachRandomTrans;
    AudioProcessorValueTreeState::SliderAttachment* sliderAttachVolume;
    AudioProcessorValueTreeState::SliderAttachment* sliderAttachReverb;

    
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
    //reverb
    Slider reverbSlider;
    Label reverbLabel;
    
    //plan to add more window choice to the grain other than han
    
    //list for sample selection
    
    //AudioThumbnail
    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    AudioThumbnailCache thumbnailCache;
    AudioThumbnail thumbnail;                          
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthAudioProcessorEditor)
};
