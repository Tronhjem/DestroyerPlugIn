/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SineWaveVisualizer.hpp"
#include "Envelope.hpp"

constexpr int HEIGHT = 600;
constexpr int WIDTH = 900;
#define DEEP_PURPLE juce::Colour(34u, 35u, 95u)
#define LIGHT_PURPLE juce::Colour(122u, 77u, 159u)
#define PINK juce::Colour(235u, 104u, 160u)
#define TURQOISE juce::Colour(168u, 218u, 205u)

class DestroyerLookAndFeelFREQ : public juce::LookAndFeel_V4
{
public:
    DestroyerLookAndFeelFREQ()
   {
       setColour (juce::Slider::thumbColourId, TURQOISE);
       setColour (juce::Slider::rotarySliderFillColourId, TURQOISE);
       setColour (juce::Slider::rotarySliderOutlineColourId, TURQOISE);
       
       setColour (juce::Slider::trackColourId, PINK);
       setColour (juce::Slider::backgroundColourId, LIGHT_PURPLE);
        
       // Text
       setColour (juce::Slider::textBoxOutlineColourId, TURQOISE);
       setColour (juce::Slider::textBoxTextColourId, TURQOISE);
       setColour (juce::Slider:: textBoxBackgroundColourId, LIGHT_PURPLE);
   }
};

class DestroyerLookAndFeelRES : public juce::LookAndFeel_V4
{
public:
    DestroyerLookAndFeelRES()
   {
        setColour (juce::Slider::thumbColourId, TURQOISE);
        setColour (juce::Slider::rotarySliderFillColourId, TURQOISE);
        setColour (juce::Slider::rotarySliderOutlineColourId, TURQOISE);
        
        setColour (juce::Slider::trackColourId, PINK);
        setColour (juce::Slider::backgroundColourId, LIGHT_PURPLE);
         
        // Text
        setColour (juce::Slider::textBoxOutlineColourId, TURQOISE);
        setColour (juce::Slider::textBoxTextColourId, TURQOISE);
        setColour (juce::Slider:: textBoxBackgroundColourId, LIGHT_PURPLE);
   }
};

//==============================================================================
/**
*/
class DestroyerAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    DestroyerAudioProcessorEditor (DestroyerAudioProcessor&);
    ~DestroyerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    Envelope envelope {100, 50, 600, 300, &sine};
private:
    void sliderValueChanged(juce::Slider *slider) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DestroyerAudioProcessor& audioProcessor;
    SineWaveVisual sine{144, 330, 512, 200, envelope};
    
    juce::Slider Freq
    {
        juce::Slider::SliderStyle::LinearVertical,
        juce::Slider::TextEntryBoxPosition::TextBoxBelow}
    ;
    
    juce::Slider Res
    {
        juce::Slider::SliderStyle::LinearVertical,
        juce::Slider::TextEntryBoxPosition::TextBoxBelow
        
    };
    DestroyerLookAndFeelFREQ sliderLookAndFeelFREQ;
    DestroyerLookAndFeelRES sliderLookAndFeelRES;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DestroyerAudioProcessorEditor)
};
