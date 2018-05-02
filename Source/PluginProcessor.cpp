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
treeState(*this, nullptr),
parameters(*this, nullptr)
#endif
{
    m_fPosition = m_fInitialPositionValue;
    m_fRandomPosition = m_fInitialRandomPositionValue;
    m_fDuration = m_fInitialDurationValue;
    m_fRandomDuration = m_fInitialRandomDurationValue;
    m_fRate = m_fInitialRateValue;
    m_fRandomRate = m_fInitialRandomRateValue;
    m_fTranspose = m_fInitialTransposeValue;
    m_fRandomTranspose = m_fInitialRandomTransposeValue;
    m_fVolume = m_fInitialVolumeValue;
    m_fReverbWet = m_fInitialReverbValue;
    
    time = 0;
    formatManager.registerBasicFormats();
    startThread();
    
    //automation
    NormalisableRange<float> positionRange(0.0001f, 1.0f);
    treeState.createAndAddParameter(pos_ID, pos_NAME, pos_NAME, positionRange, m_fInitialPositionValue, nullptr, nullptr);

    NormalisableRange<float> randPosRange(0.0f, 1.0f);
    treeState.createAndAddParameter(randPos_ID, randPos_NAME, randPos_NAME, randPosRange, m_fInitialRandomPositionValue, nullptr, nullptr);

    NormalisableRange<float> durationRange(0.0001f, 1.0f);
    treeState.createAndAddParameter(dur_ID, dur_NAME, dur_NAME, durationRange, m_fInitialDurationValue, nullptr, nullptr);

    NormalisableRange<float> randDurRange(0.0f, 1.0f);
    treeState.createAndAddParameter(randDur_ID, randDur_NAME, randDur_NAME, randDurRange, m_fInitialRandomDurationValue, nullptr, nullptr);

    NormalisableRange<float> rateRange(0.0001f, 80.0f);
    treeState.createAndAddParameter(rate_ID, rate_NAME, rate_NAME, rateRange, m_fInitialRateValue, nullptr, nullptr);

    NormalisableRange<float> randRateRange(0.0f, 1.0f);
    treeState.createAndAddParameter(randRate_ID, randRate_NAME, randRate_NAME, randRateRange, m_fInitialRandomRateValue, nullptr, nullptr);

    NormalisableRange<float> transRange(-24.0f, 24.0f);
    treeState.createAndAddParameter(trans_ID, trans_NAME, trans_NAME, transRange, m_fInitialTransposeValue, nullptr, nullptr);

    NormalisableRange<float> randTransRange(-1.0f, 1.0f);
    treeState.createAndAddParameter(randTrans_ID, randTrans_NAME, randTrans_NAME, randTransRange, m_fInitialRandomTransposeValue, nullptr, nullptr);

    NormalisableRange<float> volRange(0.0f, 1.0f);
    treeState.createAndAddParameter(vol_ID, vol_NAME, vol_NAME, volRange, m_fInitialVolumeValue, nullptr, nullptr);
    
    NormalisableRange<float> reverbRange(0.0f, 1.0f);
    treeState.createAndAddParameter(reverb_ID, reverb_NAME, reverb_NAME, reverbRange, m_fReverbWet, nullptr, nullptr);

    
    parameters.state = ValueTree("savedParams");
    
    
}

GranularSynthAudioProcessor::~GranularSynthAudioProcessor()
{
    CGrain::destroyInstance(m_pCGrain);
    m_pCGrain = NULL;
    delete m_pReverb;
    m_pReverb = NULL;
    m_ReverbParams = {};
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
    m_pReverb = new juce::Reverb();
    m_pReverb->setSampleRate(m_fSampleRateInHz);
}

void GranularSynthAudioProcessor::releaseResources()
{
    m_pReverb->reset();
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
                localStack[j]->process(buffer, *currentBuffer, buffer.getNumChannels(), numOfSamplesInBlock, numSamplesInFile, time);

            }
        }
        for(int c = 0; c < buffer.getNumChannels(); c++)
        {
            float* channelData = buffer.getWritePointer(c);
            float currentSample = channelData[i];
            channelData[i] = clipProcess(currentSample);
        }
        time++;
    }
    m_ReverbParams = m_pReverb->getParameters();
    m_ReverbParams.dryLevel = 1.0f - m_fReverbWet;
    m_ReverbParams.wetLevel = m_fReverbWet;
    m_pReverb->setParameters(m_ReverbParams);
    m_pReverb->processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), numOfSamplesInBlock);
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
    ScopedPointer<XmlElement> xml (parameters.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void GranularSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    ScopedPointer<XmlElement> theParams (getXmlFromBinary(data, sizeInBytes));
    
    if (theParams != nullptr)
    {
        if (theParams -> hasTagName(parameters.state.getType()))
        {
            parameters.state = ValueTree::fromXml(*theParams);
        }
    }
}

//==============================================================================
void GranularSynthAudioProcessor::loadAudioFile(String path)
{
    
    //    filePosition = 0;
    const File file(path);
    if (file.exists())
    {
        ScopedPointer<AudioFormatReader> reader(formatManager.createReaderFor(file));
        ReferenceCountedBuffer::Ptr newBuffer = new ReferenceCountedBuffer(file.getFileName(),
                                                                           reader->numChannels,
                                                                           (int)reader->lengthInSamples);
        if (reader != nullptr)
        {
            reader->read(newBuffer->getAudioSampleBuffer(), 0, (int)reader->lengthInSamples, 0, true, true);
            fileBuffer = newBuffer;
        }
    }
}

float GranularSynthAudioProcessor::clipProcess(float value)
{
    if (value >= 1.0f)
    {
        value = 1.0f;
    }
    if (value <= -1.0f)
    {
        value = -1.0f;
    }
    return value;
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
    
//    while (!threadShouldExit()) {
//        checkPathToOpen();
//
//        int dur = 500;
//
//        std::cout << "stack size: " << grainStack.size() << std::endl;
//        if (grainStack.size() > 0) {
//            for (int i = grainStack.size() - 1; i >= 0; i--) {
//                long long int grainEnd = grainStack[i]->getOnset() + grainStack[i]->getLength();
//                bool hasEnded = grainEnd < time;
//                if (hasEnded) {
//                    grainStack.remove(i);
//                }
//                std::cout << "hasEnded: " << hasEnded
//                << " grainEnd: " << grainEnd
//                << " time: " << time
//                << std::endl;
//            }
//        }
//
//        /** add grains */
//        if(fileBuffer != nullptr){
//            int numSamples = fileBuffer->getAudioSampleBuffer()->getNumSamples();
//            int onset = 50;
//            int length = 44100 * 0.5;
//            int startPosition  = 44100 * -500;
//            m_pCGrain->reset();
//            m_pCGrain->initInstance(time + onset, length, wrapUp(startPosition, 0, numSamples), 1, 0.3);
//            grainStack.add(m_pCGrain);
//        }
//        wait(dur);
//    }
//

    while (!threadShouldExit())
    {
        
        checkPathToOpen();
        
        //clean up grain stack
        if( grainStack.size() > 0)
        {
            for (int i = grainStack.size() - 1; i >= 0; i--)
            {
                long long int ifGrainEnds = grainStack[i]->getOnset() + grainStack[i]->getLength();
                
                if(ifGrainEnds < time)
                {
                    grainStack.remove(i);
                }
            }
        }
        
        Array<Array<int>> activedNotes;
        
        for(int i = 0; i < 128; i++)
        {
            if(midiNotes[i] > 0)
            {
                activedNotes.add(Array<int> {i, midiNotes[i]});
            }
        }
        
        if(fileBuffer != nullptr)
        {
            if(activedNotes.size() > 0)
            {
                cout<<"note pressed"<<endl;
                // initialize nextOnset
                if(nextOnset == 0)
                {
                    nextOnset = time;
                }
                
                int numOfSamples = fileBuffer->getAudioSampleBuffer()->getNumSamples();
                
              
                
                /** duration */
                
                float duration = m_fDuration * (m_fRandomDuration * (Random::getSystemRandom().nextFloat()) + 1);
                
                /** onset */
                
                long long int onset = nextOnset;
                
                /** length */
                
                float rate = m_fRate * (m_fRandomRate * (Random::getSystemRandom().nextFloat()) + 1);
                int length = rate * duration * m_fSampleRateInHz;
                
                /** start position */
                
                float randPosition = m_fRandomPosition * (Random::getSystemRandom().nextFloat());
                int startPosition = (m_fPosition + randPosition) * numOfSamples;
                startPosition = wrapUp(startPosition, 0, numOfSamples);
                
                /** transposition */
                
                // if multiple key has been pressed always count the lowest pressed key to doing transpose
                float midiNote = activedNotes[0][0];
                // C4 as the reference point for transpose
                midiNote = (midiNote - 60.0f);
                
                float trans = m_fTranspose + midiNote;
                trans = trans + (m_fRandomTranspose * (Random::getSystemRandom().nextFloat() * 24 - 12));
                
                float transRatio = pow (2.0f, trans / 12.0f);
                
                /** amplitude */
                
                float amp = m_fVolume;
                
                nextOnset = onset + (duration * m_fSampleRateInHz);
                
                CGrain::createInstance(m_pCGrain);
                
                m_pCGrain->initInstance(onset + m_delay, length, startPosition, transRatio, amp);
                
                cout<<onset<<" "<<length<<" "<<startPosition<<" "<<duration<<" "<<transRatio<<" "<<amp<<endl;
                
                grainStack.add(m_pCGrain);
                
                duration = duration + m_delay/m_fSampleRateInHz;
                
                wait(duration * 1000);
            }
            else
            {
                // reset nextOnset for no holding
                nextOnset = 0;
                wait(100);
            }
        }
        else
        {
            wait(100);
        }
        
    }
    

    
    
}

void GranularSynthAudioProcessor::processMidi(MidiBuffer& midiData)
{
    MidiBuffer::Iterator i (midiData);
    MidiMessage message;
    int samplePosition = 0;

    while(i.getNextEvent(message, samplePosition))
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
            for(int i = 0; i < 128; i++)
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
float GranularSynthAudioProcessor::getRate()
{
    return m_fRate;
}
float GranularSynthAudioProcessor::getRandomRate()
{
    return m_fRandomRate;
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
float GranularSynthAudioProcessor::getReverb()
{
    return m_fReverbWet;
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
void GranularSynthAudioProcessor::setRate(float rate)
{
    m_fRate = rate;
}
void GranularSynthAudioProcessor::setRandomRate(float randRate)
{
    m_fRandomRate = randRate;
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
void GranularSynthAudioProcessor::setReverb(float reverb)
{
    m_fReverbWet = reverb;
}


//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GranularSynthAudioProcessor();
}




