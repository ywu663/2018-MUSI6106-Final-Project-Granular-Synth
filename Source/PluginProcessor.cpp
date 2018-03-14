/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
GranularSynthAudioProcessor::GranularSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
        Thread("scheduling thread"),
        positionParam(nullptr)

#endif
{
    addParameter(positionParam = new AudioParameterFloat("pos", "Position", 0.0f, 1.0f, 0.5f));
    
    time = 0;

    formatManager.registerBasicFormats();
}

GranularSynthAudioProcessor::~GranularSynthAudioProcessor()
{
    stopThread(4000);
}

//==============================================================================
const String GranularSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GranularSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GranularSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool GranularSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double GranularSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GranularSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GranularSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GranularSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String GranularSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void GranularSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void GranularSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
}

void GranularSynthAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GranularSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GranularSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    

    ReferenceCountedBuffer::Ptr retainedBuffer(fileBuffer);
    if (retainedBuffer == nullptr) return;

    const int numSamplesInBlock = buffer.getNumSamples();
    const int numSamplesInFile = fileBuffer->getAudioSampleBuffer()->getNumSamples();
    
    const Array<Grain> localStack = grainStack;

    
    AudioSampleBuffer* currentBuffer = retainedBuffer->getAudioSampleBuffer();

    /** audio processing */
    for (int sample = 0; sample < numSamplesInBlock; sample++) {
        for (int channel = 0; channel < buffer.getNumChannels(); channel++) {
            float* outputData = buffer.getWritePointer(channel);
            const float* fileData = currentBuffer->getReadPointer(channel % currentBuffer->getNumChannels());
            outputData[sample] = fileData[filePosition];
        }

        if (filePosition < numSamplesInFile) {
            filePosition++;
        }
        else
        {
            filePosition = 0;
        }
    }
}

//==============================================================================
bool GranularSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GranularSynthAudioProcessor::createEditor()
{
    return new GranularSynthAudioProcessorEditor (*this);
}

//==============================================================================
void GranularSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void GranularSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//==============================================================================
void GranularSynthAudioProcessor::loadAudioFile(String path)
{

    filePosition = 0;
    const File file(path);
    if (file.exists()) {
        ScopedPointer<AudioFormatReader> reader(formatManager.createReaderFor(file));
        ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(file.getFileName(),
                                                                           reader->numChannels,
                                                                           (int)reader->lengthInSamples);
        if (newBuffer == nullptr) {
            std::cout << "the referenceCountedBuffer is empty!" << std::endl;
            return;
        }
        if (reader != nullptr) {
            reader->read(newBuffer->getAudioSampleBuffer(), 0, (int)reader->lengthInSamples, 0, true, true);
            std::cout << "Samples in Buffer: " << newBuffer->getAudioSampleBuffer()->getNumSamples() << std::endl;
            fileBuffer = newBuffer;
        }
    }
}

int GranularSynthAudioProcessor::wrap(int val, const int low, const int high)
{
    int range_size = high - low + 1;
    
    if (val < low)
        val += range_size * ((low - val) / range_size + 1);
    
    return low + (val - low) % range_size;
}


//==============================================================================
/** thread stuffs */
void GranularSynthAudioProcessor::run()
{
    while (!threadShouldExit()) {
        checkForPathToOpen();
        checkForBuffersToFree();
        
        // delete grains
        if( grainStack.size() > 0){
            for (int i=grainStack.size() - 1; i >= 0; --i) {
                // check if the grain has ended
                long long int grainEnd = grainStack[i].onset + grainStack[i].length;
                bool hasEnded = grainEnd < time;
                
                if(hasEnded) grainStack.remove(i); // [4]
            }
        }
        
        
        wait(500);
    }
    int numSamples = fileBuffer->getAudioSampleBuffer()->getNumSamples();
    
    int onset = 1000;
    int length = 44100;
    
    int startPosition = *positionParam * numSamples;
    startPosition = wrap(startPosition, 0, numSamples);
    
    float rate = 1;
    
    float envMid = 0.5;
    float envSus = 0.5;
    float envCurve = -3;
    
    //test
    float ratio = 0.1;
    float amp = 0.5;
    
    grainStack.add( Grain(onset, length, startPosition, envMid, envSus, envCurve, ratio, amp) );

}

void GranularSynthAudioProcessor::checkForPathToOpen()
{
    String pathToOpen;
    std::swap(pathToOpen, chosenPath);
    if (pathToOpen.isNotEmpty()) {
        loadAudioFile(pathToOpen);
    }
}

void GranularSynthAudioProcessor::checkForBuffersToFree()
{
    
}












//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GranularSynthAudioProcessor();
}
