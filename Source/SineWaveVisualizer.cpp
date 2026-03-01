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
    if (mEnvelope.mCurve != nullptr && mEnvelope.mCurve->mDirty)
    {
        FillVisual();
        mEnvelope.mCurve->mDirty = false;
    }

    g.setColour(TURQOISE);
    juce::Path p;
    
    const float halfHeight = (float)mYSize * 0.5f;
    p.startNewSubPath(0, halfHeight);
    
    for (int i = 1; i < (int)mPoints.size() - 1; ++i)
    {
        p.quadraticTo(i - 1, mPoints[i - 1] * (halfHeight-2) + halfHeight, i, mPoints[i] * (halfHeight-2) + halfHeight);
    }
    
    p.quadraticTo(511, mPoints[511] * (halfHeight - 2) + halfHeight, 512, halfHeight);
    g.strokePath(p, juce::PathStrokeType (2.0f));
    
    g.setColour(LIGHT_PURPLE);
    g.drawLine(0, halfHeight, mXSize, halfHeight);
}

void SineWaveVisual::FillSine()
{
    double t = 0;
    const double delta = 1.0 / (double)(mPoints.size());
    for (int i = 0; i < (int)mPoints.size() - 1; ++i)
    {
        mSine[i] = -1.f * sin(2.0 * 3.1415926536 * t);
        t += delta;
    }
    mSine[mPoints.size() - 1] = 0.0;
}

void SineWaveVisual::FillVisual()
{
    if (mEnvelope.mCurve == nullptr)
        return;
    
    float last = 0.f;
    const float coef = 0.5f;
    
    for (int i = 0; i < (int)mPoints.size(); ++i)
    {
        mPoints[i] = mEnvelope.mCurve->GetYValue(mSine[i]) * (1.f - coef) + last * coef;
        last = mPoints[i];
    }
}
