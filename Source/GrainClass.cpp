/*
  ==============================================================================

    GrainClass.cpp
    Created: 30 Apr 2018 3:24:00am
    Author:  Yi Wu

  ==============================================================================
*/

#include "GrainClass.h"

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

CGrain::CGrain()
{
    reset();
    m_iOnset = 0;
    m_iLength = 1000;
    m_iStartPos = 0;
    m_fTransRatio = 1.0;
    m_fAmp = 1.0;
    
}

CGrain::~CGrain()
{

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
    float c0 = y1;
    float c1 = 0.5f * (y2 - y0);
    float c2 = y0 - 2.5f * y1 + 2.f * y2 - 0.5f * y3;
    float c3 = 0.5f * (y3 - y0) + 1.5f * (y1 - y2);
    float interpolationVal = c0 + (c1 + (c2 + c3 * x) * x) * x;
    return interpolationVal;
}

Error_t CGrain::process(AudioSampleBuffer& currentBlock, AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, int fileNumSamples, long long int time)
{
    for(int channel = 0; channel < numChannels; channel++)
    {
        
        float* channelData = currentBlock.getWritePointer(channel);
        const float* fileData = fileBuffer.getReadPointer(channel);
        
        float fPosition = (int)((time - m_iOnset) * m_fTransRatio);
        int iPosition = (int)(std::ceil(fPosition));
        
        float fractionPos = fPosition - iPosition;
        
        int currentPos = iPosition + m_iStartPos;
        
        float a = fileData[(currentPos - 3) % fileNumSamples];
        float b = fileData[(currentPos - 2) % fileNumSamples];
        float c = fileData[(currentPos - 1) % fileNumSamples];
        float currentSample = fileData[currentPos % fileNumSamples];

        
        currentSample = interpolation(fractionPos, a, b, c, currentSample);
        currentSample = currentSample * m_fAmp * hanWindow(time);
        
        channelData[time % blockNumSamples] += currentSample;
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







