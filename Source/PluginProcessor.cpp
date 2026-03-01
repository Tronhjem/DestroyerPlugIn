/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout DestroyerAudioProcessor::createParameterLayout()
{
    using APF = juce::AudioParameterFloat;
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<APF>("freq", "Frequency",
        juce::NormalisableRange<float>(20.f, 20000.f, 0.1f, 0.2f), 19000.f));

    layout.add(std::make_unique<APF>("res", "Resonance",
        juce::NormalisableRange<float>(0.f, 1.f, 0.01f), 0.2f));

    auto curveRange = juce::NormalisableRange<float>(-1.f, 1.f, 0.001f);
    layout.add(std::make_unique<APF>("pt0y", "Curve Point 0 Y", curveRange, -1.0f));
    layout.add(std::make_unique<APF>("pt1x", "Curve Point 1 X", curveRange, -0.5f));
    layout.add(std::make_unique<APF>("pt1y", "Curve Point 1 Y", curveRange, -0.5f));
    layout.add(std::make_unique<APF>("pt2x", "Curve Point 2 X", curveRange,  0.0f));
    layout.add(std::make_unique<APF>("pt2y", "Curve Point 2 Y", curveRange,  0.0f));
    layout.add(std::make_unique<APF>("pt3x", "Curve Point 3 X", curveRange,  0.5f));
    layout.add(std::make_unique<APF>("pt3y", "Curve Point 3 Y", curveRange,  0.5f));
    layout.add(std::make_unique<APF>("pt4y", "Curve Point 4 Y", curveRange,  1.0f));

    auto gainRange = juce::NormalisableRange<float>(-20.f, 20.f, 0.1f);
    layout.add(std::make_unique<APF>("inGain",  "Input Gain",  gainRange, 0.f));
    layout.add(std::make_unique<APF>("outGain", "Output Gain", gainRange, 0.f));

    return layout;
}

//==============================================================================
DestroyerAudioProcessor::DestroyerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       mValueTree(*this, nullptr, "STATE", createParameterLayout())
#endif
{
    mFreqParam = mValueTree.getRawParameterValue("freq");
    mResParam  = mValueTree.getRawParameterValue("res");
    mPt0y      = mValueTree.getRawParameterValue("pt0y");
    mPt1x      = mValueTree.getRawParameterValue("pt1x");
    mPt1y      = mValueTree.getRawParameterValue("pt1y");
    mPt2x      = mValueTree.getRawParameterValue("pt2x");
    mPt2y      = mValueTree.getRawParameterValue("pt2y");
    mPt3x      = mValueTree.getRawParameterValue("pt3x");
    mPt3y      = mValueTree.getRawParameterValue("pt3y");
    mPt4y        = mValueTree.getRawParameterValue("pt4y");
    mInGainParam  = mValueTree.getRawParameterValue("inGain");
    mOutGainParam = mValueTree.getRawParameterValue("outGain");
}

DestroyerAudioProcessor::~DestroyerAudioProcessor()
{
}

//==============================================================================
const juce::String DestroyerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DestroyerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DestroyerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DestroyerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DestroyerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DestroyerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DestroyerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DestroyerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DestroyerAudioProcessor::getProgramName (int index)
{
    return {};
}

void DestroyerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DestroyerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    for (auto& filter : mMoogFilters)
        filter.setSampleRate(sampleRate);
}

void DestroyerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DestroyerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DestroyerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    const auto totalNumInputChannels = getTotalNumInputChannels();

    if (totalNumInputChannels > 2)
        return;

    // Sync curve points from parameters (supports DAW automation)
    mCurve.mPoints[0].mPosition.y = *mPt0y;
    mCurve.mPoints[1].mPosition.x = *mPt1x;
    mCurve.mPoints[1].mPosition.y = *mPt1y;
    mCurve.mPoints[2].mPosition.x = *mPt2x;
    mCurve.mPoints[2].mPosition.y = *mPt2y;
    mCurve.mPoints[3].mPosition.x = *mPt3x;
    mCurve.mPoints[3].mPosition.y = *mPt3y;
    mCurve.mPoints[4].mPosition.y = *mPt4y;

    const double freq    = static_cast<double>(*mFreqParam);
    const double res     = static_cast<double>(*mResParam);
    const float inGain   = std::pow(10.f, *mInGainParam  / 20.f);
    const float outGain  = std::pow(10.f, *mOutGainParam / 20.f);

    const int numSamples = buffer.getNumSamples();
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        for (int i = 0; i < numSamples; ++i)
        {
            float sample = channelData[i] * inGain;
            sample = mCurve.GetYValue(sample);
            channelData[i] = mMoogFilters[channel].Process((double)sample, freq, res) * outGain;
        }
    }
}

//==============================================================================
bool DestroyerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DestroyerAudioProcessor::createEditor()
{
    mEditor = new DestroyerAudioProcessorEditor (*this);
    mEditor->mEnvelope.mCurve = &mCurve;
    return mEditor;
}

//==============================================================================
void DestroyerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    const auto state = mValueTree.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DestroyerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(mValueTree.state.getType()))
        mValueTree.replaceState(juce::ValueTree::fromXml(*xml));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DestroyerAudioProcessor();
}
