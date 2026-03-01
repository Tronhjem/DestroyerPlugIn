/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DestroyerAudioProcessorEditor::DestroyerAudioProcessorEditor (DestroyerAudioProcessor& p)
    : AudioProcessorEditor (&p), mAudioProcessor (p)
{
    setSize (WIDTH, HEIGHT);
    
    addAndMakeVisible(&mEnvelope);
    addAndMakeVisible(&mSine);
    
    mEnvelope.mApvts = &p.mValueTree;
    mSine.repaint();
    
    mFreq.setLookAndFeel(&mSliderLookAndFeelFreq);
    mRes.setLookAndFeel(&mSliderLookAndFeelRes);
    mInGain.setLookAndFeel(&mSliderLookAndFeelInGain);
    mOutGain.setLookAndFeel(&mSliderLookAndFeelOutGain);

    addAndMakeVisible(&mFreq);
    mFreqAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        p.mValueTree, "freq", mFreq);
    
    addAndMakeVisible(&mRes);
    mResAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        p.mValueTree, "res", mRes);

    addAndMakeVisible(&mInGain);
    mInGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        p.mValueTree, "inGain", mInGain);

    addAndMakeVisible(&mOutGain);
    mOutGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        p.mValueTree, "outGain", mOutGain);
}

DestroyerAudioProcessorEditor::~DestroyerAudioProcessorEditor()
{
    mFreq.setLookAndFeel(nullptr);
    mRes.setLookAndFeel(nullptr);
    mInGain.setLookAndFeel(nullptr);
    mOutGain.setLookAndFeel(nullptr);
}

//==============================================================================
void DestroyerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (DEEP_PURPLE);
}

void DestroyerAudioProcessorEditor::resized()
{
    mInGain.setBounds(25, 50, 50, 200);
    mRes.setBounds(750, 300, 50, 200);
    mFreq.setBounds(750, 50, 50, 200);
    mOutGain.setBounds(820, 50, 50, 200);
}
