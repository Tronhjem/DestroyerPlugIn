/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Envelope.hpp"
#include "MoogFilter.hpp"

//
//==============================================================================
/**
*/
constexpr int MaxChannels = 2;
constexpr size_t OversampleFactor = 2;

class DestroyerAudioProcessorEditor;

class DestroyerAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    DestroyerAudioProcessor();
    ~DestroyerAudioProcessor() override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState mValueTree;
    TranslationCurve mCurve {};
    
private:
    std::array<MoogFilter, MaxChannels> mMoogFilters {MoogFilter{}, MoogFilter{}};
    DestroyerAudioProcessorEditor* mEditor = nullptr;

    std::atomic<float>* mFreqParam = nullptr;
    std::atomic<float>* mResParam = nullptr;
    std::atomic<float>* mPt0y = nullptr;
    std::atomic<float>* mPt1x = nullptr;
    std::atomic<float>* mPt1y = nullptr;
    std::atomic<float>* mPt2x = nullptr;
    std::atomic<float>* mPt2y = nullptr;
    std::atomic<float>* mPt3x = nullptr;
    std::atomic<float>* mPt3y = nullptr;
    std::atomic<float>* mPt4y = nullptr;
    std::atomic<float>* mInGainParam = nullptr;
    std::atomic<float>* mOutGainParam = nullptr;
    
    juce::dsp::Oversampling<float> mOverSampler {
        MaxChannels,
        OversampleFactor,
        juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR
    };
   
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DestroyerAudioProcessor)
};
