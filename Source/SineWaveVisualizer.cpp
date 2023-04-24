//
//  SineWaveVisualizer.cpp
//  Destroyer_StandaloneGUI - App
//
//  Created by Christian Tronhjem on 22.04.23.
//

#include "SineWaveVisualizer.hpp"
#include "PluginEditor.h"
void SineWaveVisual::paint(juce::Graphics &g)
{
    FillVisual();
    // DEBUG DRAW
//    g.drawRect(0, 0, xSize, ySize);
    
    g.setColour(TURQOISE);
    juce::Path p;
    
    p.startNewSubPath(0, 100);
    
    float halfHeight = (float)ySize * 0.5f;
    for (int i = 1; i < points.size() - 1; ++i)
    {
        // maps since to height. -2 for cleaner visual.
//        p.lineTo(i, points[i] * (halfHeight-2) + halfHeight);
        p.quadraticTo(i - 1, points[i - 1] * (halfHeight-2) + halfHeight, i, points[i] * (halfHeight-2) + halfHeight);
    }
    
    p.quadraticTo(511, points[511] * (halfHeight - 2) + halfHeight, 512, 100);
    g.strokePath(p, juce::PathStrokeType (2.0f));
    
    //middle line
    g.setColour(PINK);
    g.setColour(LIGHT_PURPLE);
    g.drawLine(0, 100, xSize, 100);
}

void SineWaveVisual::FillSine()
{
    double t = 0;
    double delta = 1.0 / (double)(points.size());
    for (int i = 0; i < points.size() - 1; ++i)
    {
        sine[i] = -1.f * sin(2.0 * 3.1415926536 * t);
        t += delta;
    }
    sine[points.size() - 1] = 0.0;
}

void SineWaveVisual::FillVisual()
{
    if (e.curve == nullptr)
        return;
    
    float last = 0.f;
    float coef = 0.5f;
    
    for (int i = 0; i < points.size(); ++i)
    {
        points[i] = e.curve->GetYValue(sine[i]) * (1.f - coef) + last * coef;
        last = points[i];
    }
//    points[points.size() - 1] = 0.f;
}
