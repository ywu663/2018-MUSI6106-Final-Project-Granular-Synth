/*
  ==============================================================================

    ReferenceCountedBuffer.h
    Created: 13 Mar 2018 9:42:00pm
    Author:  Yongliang He

  ==============================================================================
*/

#ifndef REFERENCECOUNTEDBUFFER_H_INCLUDED
#define REFERENCECOUNTEDBUFFER_H_INCLUDED

#pragma once
class ReferenceCountedBuffer: public ReferenceCountedObject
{
public:
    typedef ReferenceCountedObjectPtr<ReferenceCountedBuffer> Ptr;
    ReferenceCountedBuffer(const String& nameToUse,
                           int numChannels,
                           int numSamples)
    :   name(nameToUse),
        buffer(numChannels, numSamples)
    {
        
    }
    
    AudioSampleBuffer* getAudioSampleBuffer()
    {
        return &buffer;
    }
    
private:
    String name;
    AudioSampleBuffer buffer;
};

#endif  // REFERENCECOUNTEDBUFFER_H_INCLUDED

