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
    SineWaveVisual(int x, int y, int width, int height, Envelope& env) : ownX(x), ownY(y), e(env)
    {
        xSize = width;
        ySize = height;
        setBounds(x, y, xSize, ySize);
        FillSine();
    }
    void FillVisual();
    
private:
    void FillSine();
    void paint(juce::Graphics &g) override;
    int ownX;
    int ownY;
    int xSize;
    int ySize;
    std::array<float, 512> points;
    std::array<float, 512> sine;
    Envelope& e;
};
#endif /* SineWaveVisualizer_hpp */
