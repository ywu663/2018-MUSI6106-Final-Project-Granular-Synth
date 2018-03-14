/*
  ==============================================================================

    Grain.h
    Created: 14 Mar 2018 7:29:11am
    Author:  Yi Wu

  ==============================================================================
*/

#pragma once

  ==============================================================================
*/

#pragma once
#ifndef GRAIN_H_INCLUDED
#define GRAIN_H_INCLUDED
class Grain
{
public:
    Grain(long long int onset = 0, int length = 0, int startPos = 0)
    :   onset(onset),
        length(length),
        startPosition(startPos)
    {
    }
    ~Grain(){}
    void process(AudioSampleBuffer& currentBlock, AudioSampleBuffer& fileBuffer, int numChannels, int blockNumSamples, int fileNumSamples, long long int time)
    {
        for (int channel = 0; channel < numChannels; channel++) {
            float* outputData = currentBlock.getWritePointer(channel);
            const float* fileData = fileBuffer.getReadPointer(channel % fileBuffer.getNumChannels());
            
            long long int position = (time - onset) + startPosition;
            
//            outputData[time % blockNumSamples] += fileData[time % fileNumSamples];
            outputData[time % blockNumSamples] = fileData[position % fileNumSamples];
        }
    }
    
    
    /** public members */
    long long int onset;
    int length;
    int startPosition;
};


#endif  // GRAIN_H_INCLUDED
