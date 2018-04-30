/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ReferenceCountedBuffer.h"
#include "GrainClass.h"


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
    void checkForPathToOpen();
    
    
    
    String chosenPath;  //!< file chosen to open
    
    
    
private:
    //==============================================================================
    /** private members declaration */
    AudioFormatManager formatManager;
    ReferenceCountedBuffer::Ptr fileBuffer;
    //    int filePosition;   //!< read pointer of the buffer storing the audio file content
    
    long long int time;
    
//    CGrain grain;
//    Array<CGrain> grainStack;
    
    CGrain *m_pCGrain = NULL;
    Array<CGrain*> grainStack;
    
    
    /** private functions declaration */
    void loadAudioFile(String);
    int wrap(int, const int, const int);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GranularSynthAudioProcessor)
};
