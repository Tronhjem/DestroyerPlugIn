/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DestroyerAudioProcessorEditor::DestroyerAudioProcessorEditor (DestroyerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (WIDTH, HEIGHT);
    
    addAndMakeVisible(&envelope);
    addAndMakeVisible(&sine);
    
    
    Freq.setBounds(750, 50, 50, 200);
    Freq.setRange(20.0, 20000.0, 0.1);
    Freq.setValue(19000.0);
    Freq.setNormalisableRange(juce::NormalisableRange<double>(20.0, 20000.0, 0.1, 0.2));
    Freq.setLookAndFeel(&sliderLookAndFeelFREQ);
    Freq.addListener(this);
    addAndMakeVisible(&Freq);
    
    Res.setBounds(750, 300, 50, 200);
    Res.setRange(0.0, 1.0, 0.01);
    Res.setValue(0.2);
    Res.setLookAndFeel(&sliderLookAndFeelRES);
    Res.addListener(this);
    addAndMakeVisible(&Res);
}

DestroyerAudioProcessorEditor::~DestroyerAudioProcessorEditor()
{
}

void DestroyerAudioProcessorEditor::sliderValueChanged(juce::Slider* slider)
{
    if(slider == &Freq)
    {
        audioProcessor.Freq = slider->getValue();
    }
    else if (slider == &Res)
    {
        audioProcessor.Res = slider->getValue();
    }
}

//==============================================================================
void DestroyerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (DEEP_PURPLE);
}

void DestroyerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
