/*
  ==============================================================================

    GrainClass.h
    Created: 30 Apr 2018 3:24:00am
    Author:  Yi Wu

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ErrorDef.h"
#include <cmath>
#include <iostream>

using namespace std;


#pragma once

#ifndef GRAINCLASS_H_INCLUDED
#define GRAINCLASS_H_INCLUDED

class CGrain
{
public:
    CGrain();
    CGrain(long long int onset, int length, int startPos, float transRatio, float amp);
    ~CGrain();
    static Error_t createInstance(CGrain*& pCGrain);
    static Error_t destroyInstance(CGrain*& pCGrain);
    Error_t initInstance(long long int onset, int length, int startPos, float transRatio, float amp);
    Error_t reset();
    float hanWindow(long long int time);
    float interpolation(float x, float y0, float y1, float y2, float y3);
    Error_t process (AudioSampleBuffer& currentBlock, AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, int fileNumSamples, long long int time);
    long long int getOnset();
    int getLength();
    int getStartPos();
    
private:
    long long int m_iOnset;
    int m_iLength;
    int m_iStartPos;
    float m_fTransRatio;
    float m_fAmp;
    
};








#endif  // GRAINCLASS_H_INCLUDED
