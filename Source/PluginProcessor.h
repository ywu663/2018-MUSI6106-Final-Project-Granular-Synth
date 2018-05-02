/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ReferenceCountedBuffer.h"
#include <algorithm>
#include <iostream>
#include "GrainClass.h"

using namespace std;

//==============================================================================
/**
 */
class GranularSynthAudioProcessor  : public AudioProcessor,
public Thread
{
public:
    //==============================================================================
    GranularSynthAudioProcessor();
    ~GranularSynthAudioProcessor();
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    
#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif
    
    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;
    
    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    
    //==============================================================================
    const String getName() const override;
    
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    void processMidi(MidiBuffer& midiData);
    double getTailLengthSeconds() const override;
    
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;
    
    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //==============================================================================
    /** thread stuffs */
    void run() override;
    void checkPathToOpen();
    
    String chosenPath;  //!< file chosen to open
    
    //helper functions
    float getPosition();
    float getRandomPosition();
    float getDuration();
    float getRandomDuration();
    float getDensity();
    float getRandomDensity();
    float getTranspose();
    float getRandomTranspose();
    float getVolume();
    
    void setPosition(float position);
    void setRandomPosition(float randPosition);
    void setDuration(float duration);
    void setRandomDuration(float randDuration);
    void setDensity(float density);
    void setRandomDensity(float randDensity);
    void setTranspose(float transpose);
    void setRandomTranspose(float randTranspose);
    void setVolume(float volume);
    
    //automation
    AudioProcessorValueTreeState treeState;
    AudioProcessorValueTreeState parameters;
    
    
private:
    //==============================================================================
    /** private members declaration */
    AudioFormatManager formatManager;
    ReferenceCountedBuffer::Ptr fileBuffer;
    //    int filePosition;   //!< read pointer of the buffer storing the audio file content
    
    long long int time;
    long long int nextOnset;

    
    CGrain *m_pCGrain = NULL;
    Array<CGrain*> grainStack;
    
    
    /** private functions declaration */
    void loadAudioFile(String);
    float clipProcess(float value);
    int wrapUp(int value, int lower, int upper);
    
    
    //grain parameters
    float m_fPosition = 0.0;
    float m_fRandomPosition = 0.0;
    float m_fDuration = 0.0;
    float m_fRandomDuration = 0.0;
    float m_fDensity = 0.0;
    float m_fRandomDensity = 0.0;
    float m_fTranspose = 0.0;
    float m_fRandomTranspose = 0.0;
    float m_fVolume = 0.0;
    //delay to prevent glitching sound
    const float m_delay = 500.0;

    //processing parameters
    int midiNotes[128] = {};
    float m_fSampleRateInHz = 0.0;
    
    //initial plugin parameter values
    const float m_fInitialPositionValue = 0.5;
    const float m_fInitialRandomPositionValue = 0.0;
    const float m_fInitialDurationValue = 0.2;
    const float m_fInitialRandomDurationValue = 0.0;
    const float m_fInitialDensityValue = 20.0;
    const float m_fInitialRandomDensityValue = 0.0;
    const float m_fInitialTransposeValue = 0;
    const float m_fInitialRandomTransposeValue = 0.0;
    const float m_fInitialVolumeValue = 0.5;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthAudioProcessor)
};
