//
//  SineWaveVisualizer.hpp
//  Destroyer_StandaloneGUI - App
//
//  Created by Christian Tronhjem on 22.04.23.
//

#ifndef SineWaveVisualizer_hpp
#define SineWaveVisualizer_hpp
#include <JuceHeader.h>
#include "Envelope.hpp"

class SineWaveVisual : public juce::Component
{
public:
    SineWaveVisual(int x, int y, int width, int height, Envelope& env) : mOwnX(x), mOwnY(y), mEnvelope(env)
    {
        mXSize = width;
        mYSize = height;
        setBounds(x, y, mXSize, mYSize);
        FillSine();
    }
    void FillVisual();
    
private:
    void FillSine();
    void paint(juce::Graphics &g) override;
    int mOwnX;
    int mOwnY;
    int mXSize;
    int mYSize;
    std::array<float, 512> mPoints;
    std::array<float, 512> mSine;
    Envelope& mEnvelope;
};
#endif /* SineWaveVisualizer_hpp */
