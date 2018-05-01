/*
  ==============================================================================

    GrainClass.cpp
    Author:  Yi Wu

  ==============================================================================
*/

#include "GrainClass.h"


CGrain::CGrain(): m_iOnset(0),m_iLength(1000),m_iStartPos(0),m_fTransRatio(1.0),m_fAmp(1.0)
{
    this->reset();
}

CGrain::CGrain(long long int onset, int length, int startPos, float transRatio, float amp):   m_iOnset(onset),m_iLength(length),m_iStartPos(startPos), m_fTransRatio(transRatio), m_fAmp(amp)
{
    
}

CGrain::~CGrain()
{
    this->reset();
}


Error_t CGrain::createInstance(CGrain*& pCGrain)
{
    pCGrain = new CGrain ();
    
    if (!pCGrain)
        return kUnknownError;
    
    return kNoError;
}

Error_t CGrain::destroyInstance(CGrain*& pCGrain)
{
    if (!pCGrain)
        return kUnknownError;
    
    delete pCGrain;
    pCGrain = 0;
    
    return kNoError;
}


Error_t CGrain::initInstance(long long int onset, int length, int startPos, float transRatio, float amp)
{
    m_iOnset = onset;
    m_iLength = length;
    m_iStartPos = startPos;
    m_fTransRatio = transRatio;
    m_fAmp = amp;
    return kNoError;
}

Error_t CGrain::reset()
{
    m_iOnset = 0;
    m_iLength = 0;
    m_iStartPos = 0;
    m_fTransRatio = 0.0;
    m_fAmp = 0.0;
    return kNoError;
}


float CGrain::hanWindow(long long int time)
{
    float gain = 0.0;
    float windowPosition = ((time - m_iOnset) / (float)m_iLength);
    gain = 0.5 * (1 - cos(2*M_PI*windowPosition));
    return gain;
}


float CGrain::interpolation(float x, float y0, float y1, float y2, float y3)
{
    float z0 = y1;
    float z1 = 0.5f * (y2 - y0);
    float z2 = y0 - 2.5f * y1 + 2.f * y2 - 0.5f * y3;
    float z3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);
    float interpolationVal = z0 + (z1 + (z2 + z3 * x) * x) * x;
    return interpolationVal;
}

Error_t CGrain::process(AudioSampleBuffer& currentBlock, AudioSampleBuffer& fileBuffer, int numOfChannels, int blockOfNumSamples, int fileNumOfSamples, long long int time)
{

    
    for (int c = 0; c < numOfChannels; c++)
    {
        float* outputData = currentBlock.getWritePointer(c);
        const float* fileData = fileBuffer.getReadPointer(c % fileBuffer.getNumChannels());

//        long long int position = (time - m_iOnset) + m_iStartPos;
//
//                    outputData[time % blockNumSamples] += fileData[time % fileNumSamples];
        //outputData[time % blockNumSamples] = fileData[position % fileNumSamples];
        
        
        float fPosition = (int)((time - m_iOnset) * m_fTransRatio);
        int iPosition = (int)(std::ceil(fPosition));
        
        float fractionPos = fPosition - iPosition;
        
        int currentPos = iPosition + m_iStartPos;
        
        float x = fileData[(currentPos - 3) % fileNumOfSamples];
        float y = fileData[(currentPos - 2) % fileNumOfSamples];
        float z = fileData[(currentPos - 1) % fileNumOfSamples];
        float currentSample = fileData[currentPos % fileNumOfSamples];
        
        
        currentSample = interpolation(fractionPos, x, y, z, currentSample);
        currentSample = currentSample * m_fAmp * hanWindow(time);
        
        outputData[time % blockOfNumSamples] += currentSample;
    }
    return kNoError;
}



long long int CGrain::getOnset()
{
    return m_iOnset;
}
int CGrain::getLength()
{
    return m_iLength;
}
int CGrain::getStartPos()
{
    return m_iStartPos;
}







