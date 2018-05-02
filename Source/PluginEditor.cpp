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
: AudioProcessorEditor (&p), processor (p), thumbnailCache (5), thumbnail (256, formatManager, thumbnailCache)
{
    LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName ("Helvetica");
    
    //timer
    startTimerHz(100);
    
    //thumbnail
    formatManager.registerBasicFormats();
    thumbnail.addChangeListener(this);
    
    addAndMakeVisible(openButton);
    openButton.setButtonText("Open...");
    openButton.addListener(this);
    setSize (900, 720);
    
    
    //Automation
    sliderAttachPosition = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, pos_ID, positionSlider);
    sliderAttachRandomPosition = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, randPos_ID, randomPositionSlider);
    sliderAttachDuration = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, dur_ID, durationSlider);
    sliderAttachRandomDuration = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, randDur_ID, randomDurationSlider);
    sliderAttachDensity = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, den_ID, densitySlider);
    sliderAttachRandomDensity = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, randDen_ID, randomDensitySlider);
    sliderAttachTranspose = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, trans_ID, transposeSlider);
    sliderAttachRandomTrans = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, randTrans_ID, randomTransposeSlider);
    sliderAttachVolume = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, vol_ID, volumeSlider);
    sliderAttachReverb = new AudioProcessorValueTreeState::SliderAttachment(processor.treeState, reverb_ID, reverbSlider);

    
    //position
    addAndMakeVisible(positionSlider);
    positionSlider.setRange(0.0001f, 1.0f, 0.01);
    positionSlider.setColour( 0x1001500 , Colour(39, 50, 56));
    positionSlider.setColour( 0x1001700 , Colour(39, 50, 56));
    positionSlider.addListener(this);
    addAndMakeVisible(positionLabel);
    positionLabel.setJustificationType(juce::Justification::centredLeft);
    positionLabel.setText("Position", dontSendNotification);
    //random position
    addAndMakeVisible(randomPositionSlider);
    randomPositionSlider.setRange(0.0f, 1.0f, 0.01);
    randomPositionSlider.setColour( 0x1001500 , Colour(39, 50, 56));
    randomPositionSlider.setColour( 0x1001700 , Colour(39, 50, 56));
    randomPositionSlider.addListener(this);
    addAndMakeVisible(randomPositionLabel);
    randomPositionLabel.setJustificationType(juce::Justification::centredLeft);
    randomPositionLabel.setText("Random Position", dontSendNotification);
    //duration
    addAndMakeVisible(durationSlider);
    durationSlider.setRange(0.0001f, 1.0f, 0.001);
    durationSlider.setColour( 0x1001500 , Colour(39, 50, 56));
    durationSlider.setColour( 0x1001700 , Colour(39, 50, 56));
    durationSlider.addListener(this);
    addAndMakeVisible(durationLabel);
    durationLabel.setJustificationType(juce::Justification::centredLeft);
    durationLabel.setText("Duration", dontSendNotification);
    //random duration
    addAndMakeVisible(randomDurationSlider);
    randomDurationSlider.setRange(0.0f, 1.0f, 0.001);
    randomDurationSlider.setColour( 0x1001500 , Colour(39, 50, 56));
    randomDurationSlider.setColour( 0x1001700 , Colour(39, 50, 56));
    randomDurationSlider.addListener(this);
    addAndMakeVisible(randomDurationLabel);
    randomDurationLabel.setJustificationType(juce::Justification::centredLeft);
    randomDurationLabel.setText("Random Duration", dontSendNotification);
    //density
    addAndMakeVisible(densitySlider);
    densitySlider.setRange(0.0001f, 80.0f, 0.001);
    densitySlider.setColour( 0x1001500 , Colour(39, 50, 56));
    densitySlider.setColour( 0x1001700 , Colour(39, 50, 56));
    densitySlider.addListener(this);
    addAndMakeVisible(densityLabel);
    densityLabel.setJustificationType(juce::Justification::centredLeft);
    densityLabel.setText("Density", dontSendNotification);
    //random density
    addAndMakeVisible(randomDensitySlider);
    randomDensitySlider.setRange(0.0f, 1.0f, 0.001);
    randomDensitySlider.setColour( 0x1001500 , Colour(39, 50, 56));
    randomDensitySlider.setColour( 0x1001700 , Colour(39, 50, 56));
    randomDensitySlider.addListener(this);
    addAndMakeVisible(randomDensityLabel);
    randomDensityLabel.setJustificationType(juce::Justification::centredLeft);
    randomDensityLabel.setText("Random Density", dontSendNotification);
    //transpose
    addAndMakeVisible(transposeSlider);
    transposeSlider.setRange(-24.0f, 24.0f, 0.01);
    transposeSlider.setColour( 0x1001500 , Colour(39, 50, 56));
    transposeSlider.setColour( 0x1001700 , Colour(39, 50, 56));
    transposeSlider.addListener(this);
    addAndMakeVisible(transposeLabel);
    transposeLabel.setJustificationType(juce::Justification::centredLeft);
    transposeLabel.setText("Transposition", dontSendNotification);
    //random transpose
    addAndMakeVisible(randomTransposeSlider);
    randomTransposeSlider.setRange(-1.0f, 1.0f, 0.001);
    randomTransposeSlider.setColour( 0x1001500 , Colour(39, 50, 56));
    randomTransposeSlider.setColour( 0x1001700 , Colour(39, 50, 56));
    randomTransposeSlider.addListener(this);
    addAndMakeVisible(randomTransposeLabel);
    randomTransposeLabel.setJustificationType(juce::Justification::centredLeft);
    randomTransposeLabel.setText("Random Trans", dontSendNotification);
    //volume
    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0.0f, 1.0f, 0.001);
    volumeSlider.setColour( 0x1001500 , Colour(39, 50, 56));
    volumeSlider.setColour( 0x1001700 , Colour(39, 50, 56));
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeLabel);
    volumeLabel.setJustificationType(juce::Justification::centredLeft);
    volumeLabel.setText("Volume", dontSendNotification);
    //reverb
    addAndMakeVisible(reverbSlider);
    reverbSlider.setRange(0.0f, 1.0f, 0.001);
    reverbSlider.setColour( 0x1001500 , Colour(39, 50, 56));
    reverbSlider.setColour( 0x1001700 , Colour(39, 50, 56));
    reverbSlider.addListener(this);
    addAndMakeVisible(reverbLabel);
    reverbLabel.setJustificationType(juce::Justification::centredLeft);
    reverbLabel.setText("Reverb", dontSendNotification);
    
}

GranularSynthAudioProcessorEditor::~GranularSynthAudioProcessorEditor()
{
    delete sliderAttachPosition;
    sliderAttachPosition = NULL;
    delete sliderAttachRandomPosition;
    sliderAttachRandomPosition = NULL;
    delete sliderAttachDuration;
    sliderAttachDuration = NULL;
    delete sliderAttachRandomDuration;
    sliderAttachRandomDuration = NULL;
    delete sliderAttachDensity;
    sliderAttachDensity = NULL;
    delete sliderAttachRandomDensity;
    sliderAttachRandomDensity = NULL;
    delete sliderAttachTranspose;
    sliderAttachTranspose = NULL;
    delete sliderAttachRandomTrans;
    sliderAttachRandomTrans = NULL;
    delete sliderAttachVolume;
    sliderAttachVolume = NULL;
    delete sliderAttachVolume;
    sliderAttachVolume = NULL;
    delete sliderAttachReverb;
    sliderAttachReverb = NULL;
}

//==============================================================================
void GranularSynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::white);
    g.setFont (Font(30.0f, 1));
    g.drawFittedText ("THE RAIN", 30, 30, 200, 20, Justification::centredLeft, 1);
    g.setFont (Font(15.0f, 1));
    g.drawFittedText ("v1.0", 180, 35, 200, 20, Justification::centredLeft, 1);
    g.setFont (Font(15.0f, 0));
    g.drawFittedText ("Granular Texture Synthesizer", 225, 35, 200, 20, Justification::centredLeft, 1);
    g.setColour(Colour(77, 80, 87));
    g.fillRoundedRectangle(10 , 80, getWidth() - 20 , getHeight() - 90, 5.0f);

    
    
    Rectangle<int> thumbnailBounds (30, 100, getWidth() - 60, 180);
    if (thumbnail.getNumChannels() == 0)
        paintIfNoFileLoaded (g, thumbnailBounds);
    else
    {
        paintIfFileLoaded (g, thumbnailBounds);
        paintPlayHead (g, thumbnailBounds);
    }
    
    
}

void GranularSynthAudioProcessorEditor::changeListenerCallback (ChangeBroadcaster* source)
{
    thumbnailChanged();
}

void GranularSynthAudioProcessorEditor::thumbnailChanged()
{
    repaint();
}

void GranularSynthAudioProcessorEditor::paintIfNoFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds)
{
    g.setColour (Colour (24, 31, 34));
    g.fillRoundedRectangle(30, 100, getWidth() - 60, 180, 5.0f);
    g.setColour (Colours::white);
    g.drawFittedText ("No File Loaded", thumbnailBounds, Justification::centred, 1.0f);
}

void GranularSynthAudioProcessorEditor::paintIfFileLoaded (Graphics& g, const Rectangle<int>& thumbnailBounds)
{
    g.setColour (Colour (24, 31, 34));
    g.fillRoundedRectangle(30, 100, getWidth() - 60, 180, 5.0f);
    g.setColour (Colour (228, 161, 97));
    thumbnail.drawChannels (g,
                            thumbnailBounds,
                            0.0,
                            thumbnail.getTotalLength(),
                            2.5f);
}

void GranularSynthAudioProcessorEditor::paintPlayHead (Graphics& g, const Rectangle<int>& thumbnailBounds)
{
    g.setColour (Colours::white);
    g.drawLine (processor.getPosition() * (getWidth() - 60) + thumbnailBounds.getX(), thumbnailBounds.getY(), processor.getPosition()  * (getWidth() - 60) + thumbnailBounds.getX(),
                thumbnailBounds.getBottom(), 1.0f);
}


void GranularSynthAudioProcessorEditor::resized()
{
    int linespace = 40;
    const int border = 150;
    int startHeight = 270;
    openButton.setBounds(getWidth() - 100, 45, 80, 20);
    positionSlider.setBounds(10 + border, startHeight + linespace, getWidth() - 35 - border, 20);
    positionLabel.setBounds(30, startHeight + linespace, 10 + border, 20);
    randomPositionSlider.setBounds(10 + border, startHeight + 2 * linespace, getWidth() - 35 - border, 20);
    randomPositionLabel.setBounds(30, startHeight + 2 * linespace, 10 + border, 20);
    durationSlider.setBounds(10 + border, startHeight + 3 * linespace, getWidth() - 35 - border, 20);
    durationLabel.setBounds(30, startHeight + 3 * linespace, 10 + border, 20);
    randomDurationSlider.setBounds(10 + border, startHeight + 4 * linespace, getWidth() - 35 - border, 20);
    randomDurationLabel.setBounds(30, startHeight + 4 * linespace, 10 + border, 20);
    densitySlider.setBounds(10 + border, startHeight + 5 * linespace, getWidth() - 35 - border, 20);
    densityLabel.setBounds(30, startHeight + 5 * linespace, 10 + border, 20);
    randomDensitySlider.setBounds(10 + border, startHeight + 6 * linespace, getWidth() - 35 - border, 20);
    randomDensityLabel.setBounds(30, startHeight + 6 * linespace, 10 + border, 20);
    transposeSlider.setBounds(10 + border, startHeight + 7 * linespace, getWidth() - 35 - border, 20);
    transposeLabel.setBounds(30, startHeight + 7 * linespace, 10 + border, 20);
    randomTransposeSlider.setBounds(10 + border, startHeight + 8 * linespace, getWidth() - 35 - border, 20);
    randomTransposeLabel.setBounds(30, startHeight + 8 * linespace, 10 + border, 20);
    volumeSlider.setBounds(10 + border, startHeight + 9 * linespace, getWidth() - 35 - border, 20);
    volumeLabel.setBounds(30, startHeight + 9 * linespace, 10 + border, 20);
    reverbSlider.setBounds(10 + border, startHeight + 10 * linespace, getWidth() - 35 - border, 20);
    reverbLabel.setBounds(30, startHeight + 10 * linespace, 10 + border, 20);
}

void GranularSynthAudioProcessorEditor::timerCallback()
{

}



//==============================================================================
/** button stuffs */
void GranularSynthAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &openButton) {
        openButtonClicked();
    }
}

/** slider stuffs */
void GranularSynthAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    
    if (slider == &positionSlider)
    {
        repaint();
        processor.setPosition((float)slider->getValue());
        cout<<"position: "<<processor.getPosition()<<endl;
    }
    else if (slider == &randomPositionSlider)
    {
        processor.setRandomPosition((float)slider->getValue());
        cout<<"random position: "<<processor.getRandomPosition()<<endl;
    }
    else if (slider == &durationSlider)
    {
        processor.setDuration((float)slider->getValue());
        cout<<"duration: "<<processor.getDuration()<<endl;
    }
    else if (slider == &randomDurationSlider)
    {
        processor.setRandomDuration((float)slider->getValue());
        cout<<"random duration: "<<processor.getRandomDuration()<<endl;
    }
    else if (slider == &densitySlider)
    {
        processor.setDensity((float)slider->getValue());
        cout<<"density: "<<processor.getDensity()<<endl;
    }
    else if (slider == &randomDensitySlider)
    {
        processor.setRandomDensity((float)slider->getValue());
        cout<<"random density: "<<processor.getRandomDensity()<<endl;
    }
    else if (slider == &transposeSlider)
    {
        processor.setTranspose((float)slider->getValue());
        cout<<"transpose: "<<processor.getTranspose()<<endl;
    }
    else if (slider == &randomTransposeSlider)
    {
        processor.setRandomTranspose((float)slider->getValue());
        cout<<"random transpose: "<<processor.getRandomTranspose()<<endl;
    }
    else if (slider == &volumeSlider)
    {
        processor.setVolume((float)slider->getValue());
        cout<<"volume: "<<processor.getVolume()<<endl;
    }
    else if (slider == &reverbSlider)
    {
        processor.setReverb((float)slider->getValue());
        cout<<"reverb: "<<processor.getReverb()<<endl;
    }
    
}

void GranularSynthAudioProcessorEditor::openButtonClicked()
{
    FileChooser chooser ("Select a File to open...",
                         File::nonexistent,
                         "*.wav");
    
    if(chooser.browseForFileToOpen())
    {
        const File file (chooser.getResult());
        auto* reader = formatManager.createReaderFor (file);
        String path (file.getFullPathName());
        std::swap(processor.chosenPath, path);
        //thumbnail
        if (reader != nullptr)
        {
            std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
            transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
            thumbnail.setSource (new FileInputSource (file));
            readerSource.reset (newSource.release());

        }
    }
}




//==============================================================================
/** private functions definition */
