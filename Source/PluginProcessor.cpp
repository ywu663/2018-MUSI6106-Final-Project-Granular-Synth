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
Thread("scheduling thread")
#endif
{
    CGrain::createInstance(m_pCGrain);
    m_pCGrain->reset();
    
    time = 0;
    formatManager.registerBasicFormats();
    startThread();
}

GranularSynthAudioProcessor::~GranularSynthAudioProcessor()
{
    CGrain::destroyInstance(m_pCGrain);
    m_pCGrain = NULL;
    stopThread(4000);
}

//==============================================================================
const String GranularSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
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
    m_fSampleRateInHz = sampleRate;
}

void GranularSynthAudioProcessor::releaseResources()
{
}


bool GranularSynthAudioProcessor::acceptsMidi() const
{

    return true;

}

bool GranularSynthAudioProcessor::producesMidi() const
{

    return false;

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
    int numOfSamplesInBlock = buffer.getNumSamples();
    processMidi(midiMessages);

    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); i++)
        buffer.clear (i, 0, numOfSamplesInBlock);
    
    /** if no file opened */
    ReferenceCountedBuffer::Ptr retainedBuffer(fileBuffer);
    if (retainedBuffer == nullptr)
        return;
    
    AudioSampleBuffer* currentBuffer = retainedBuffer->getAudioSampleBuffer();
    const int numSamplesInFile = fileBuffer->getAudioSampleBuffer()->getNumSamples();
    
    /** define local thread for thread safe */
    const Array<CGrain*> localStack = grainStack;
    
    /** audio processing */
    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); i++)
    {
        buffer.clear(i, 0, numOfSamplesInBlock);
    }
    for (int i = 0; i < numOfSamplesInBlock; i++)
    {
        for(int j = 0; j < localStack.size(); j++)
        {
            if (localStack[j]->getOnset() < time)
            {
                if (time < (localStack[j]->getOnset() + localStack[j]->getLength()))
                {
                    localStack[j]->process(buffer, *currentBuffer, buffer.getNumChannels(), numOfSamplesInBlock, numSamplesInFile, time);
                }
            }
        }
        for(int c = 0; c < buffer.getNumChannels(); c++)
        {
            float* channelData = buffer.getWritePointer(c);
            float currentSample = channelData[i];
            channelData[i] = clipProcess(currentSample, -1.0, 1.0);
        }
        time++;
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
    
    //    filePosition = 0;
    const File file(path);
    if (file.exists()) {
        ScopedPointer<AudioFormatReader> reader(formatManager.createReaderFor(file));
        ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(file.getFileName(),
                                                                           reader->numChannels,
                                                                           (int)reader->lengthInSamples);
        if (reader != nullptr) {
            reader->read(newBuffer->getAudioSampleBuffer(), 0, (int)reader->lengthInSamples, 0, true, true);
            fileBuffer = newBuffer;
        }
    }
}

float GranularSynthAudioProcessor::clipProcess(float value, float lower, float upper)
{
    float min = std::min(value, upper);
    return std::max(min, lower);
}

int GranularSynthAudioProcessor::wrapUp(int value, int lower, int upper)
{
    while (value < lower)
    {
        value = value + (upper - lower + 1);
    }
    return lower + ((value - lower) % (upper - lower + 1));
}


//==============================================================================
/** thread stuffs */
void GranularSynthAudioProcessor::run()
{
    while (!threadShouldExit()) {
        checkPathToOpen();

        int dur = 500;

        std::cout << "stack size: " << grainStack.size() << std::endl;
        if (grainStack.size() > 0) {
            for (int i = grainStack.size() - 1; i >= 0; i--) {
                long long int grainEnd = grainStack[i]->getOnset() + grainStack[i]->getLength();
                bool hasEnded = grainEnd < time;
                if (hasEnded) {
                    grainStack.remove(i);
                }
                std::cout << "hasEnded: " << hasEnded
                << " grainEnd: " << grainEnd
                << " time: " << time
                << std::endl;
            }
        }

        /** add grains */
        if(fileBuffer != nullptr){
            int numSamples = fileBuffer->getAudioSampleBuffer()->getNumSamples();
            int onset = 50;
            int length = 44100 * 0.5;
            int startPosition  = 44100 * -500;
            m_pCGrain->reset();
            m_pCGrain->initInstance(time + onset, length, wrapUp(startPosition, 0, numSamples), 1, 0.3);
            grainStack.add(m_pCGrain);
        }
        wait(dur);
    }

    

    
    
    
    
    
    
}

void GranularSynthAudioProcessor::processMidi(MidiBuffer& midiData)
{
    MidiBuffer::Iterator i (midiData);
    MidiMessage message;
    int counter = 0;

    while(i.getNextEvent(message, counter))
    {
        if(message.isNoteOn())
        {
            midiNotes[message.getNoteNumber()] = message.getVelocity();
            cout<<"nice!"<<endl;
        }
        if(message.isNoteOff())
        {
            midiNotes[message.getNoteNumber()] = 0;
        }
        if(message.isAllNotesOff())
        {
            for(int i = 0; i < 128; ++i)
                midiNotes[i] = 0;
        }
    }
}



void GranularSynthAudioProcessor::checkPathToOpen()
{
    String pathToOpen;
    std::swap(pathToOpen, chosenPath);
    if (pathToOpen.isNotEmpty()) {
        loadAudioFile(pathToOpen);
    }
}

float GranularSynthAudioProcessor::getPosition()
{
    return m_fPosition;
}
float GranularSynthAudioProcessor::getRandomPosition()
{
    return m_fRandomPosition;
}
float GranularSynthAudioProcessor::getDuration()
{
    return m_fDuration;
}
float GranularSynthAudioProcessor::getRandomDuration()
{
    return m_fRandomDuration;
}
float GranularSynthAudioProcessor::getDensity()
{
    return m_fDensity;
}
float GranularSynthAudioProcessor::getRandomDensity()
{
    return m_fRandomDensity;
}
float GranularSynthAudioProcessor::getTranspose()
{
    return m_fTranspose;
}
float GranularSynthAudioProcessor::getRandomTranspose()
{
    return m_fRandomTranspose;
}
float GranularSynthAudioProcessor::getVolume()
{
    return m_fVolume;
}



void GranularSynthAudioProcessor::setPosition(float position)
{
    m_fPosition = position;
}
void GranularSynthAudioProcessor::setRandomPosition(float randPosition)
{
    m_fRandomPosition = randPosition;
}
void GranularSynthAudioProcessor::setDuration(float duration)
{
    m_fDuration = duration;
}
void GranularSynthAudioProcessor::setRandomDuration(float randDuration)
{
    m_fRandomDuration = randDuration;
}
void GranularSynthAudioProcessor::setDensity(float density)
{
    m_fDensity = density;
}
void GranularSynthAudioProcessor::setRandomDensity(float randDensity)
{
    m_fRandomDensity = randDensity;
}
void GranularSynthAudioProcessor::setTranspose(float transpose)
{
    m_fTranspose = transpose;
}
void GranularSynthAudioProcessor::setRandomTranspose(float randTranspose)
{
    m_fRandomTranspose = randTranspose;
}
void GranularSynthAudioProcessor::setVolume(float volume)
{
    m_fVolume = volume;
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GranularSynthAudioProcessor();
}




