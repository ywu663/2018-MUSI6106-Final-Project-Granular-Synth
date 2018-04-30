/*
 ==============================================================================
 This file was auto-generated!
 It contains the basic framework code for a JUCE plugin editor.
 ==============================================================================
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
GranularSynthAudioProcessorEditor::GranularSynthAudioProcessorEditor (GranularSynthAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p)
{
    addAndMakeVisible(openButton);
    openButton.setButtonText("Open...");
    openButton.addListener(this);
    setSize (400, 300);
}

GranularSynthAudioProcessorEditor::~GranularSynthAudioProcessorEditor()
{
}

//==============================================================================
void GranularSynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    
    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void GranularSynthAudioProcessorEditor::resized()
{
    openButton.setBounds(10, 10, 120, 20);
}

//==============================================================================
/** button stuffs */
void GranularSynthAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &openButton) {
        openButtonClicked();
    }
}

void GranularSynthAudioProcessorEditor::openButtonClicked()
{
    FileChooser chooser ("Select a File to open...", // 1
                         File::nonexistent,
                         "*.wav", "*.aif", "*.aiff");
    
    if(chooser.browseForFileToOpen()){  // 2
        const File file (chooser.getResult());
        String path (file.getFullPathName());
        std::swap(processor.chosenPath, path);
    }
}




//==============================================================================
/** private functions definition */
