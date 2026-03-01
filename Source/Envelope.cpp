//
//  Envelope.cpp
//  Destroyer_StandaloneGUI - App
//
//  Created by Christian Tronhjem on 22.04.23.
//

#include "Envelope.hpp"
#include "SineWaveVisualizer.hpp"
#include "PluginEditor.h"

void Envelope::paint(juce::Graphics &g)
{
    g.setColour(LIGHT_PURPLE);
    g.drawLine(mEnvelopeMargin, mBoundsEndPosY / 2, mBoundsEndPosX - mEnvelopeMargin, mBoundsEndPosY / 2);
    g.drawLine(mBoundsEndPosX / 2, mEnvelopeMargin, mBoundsEndPosX / 2, mBoundsEndPosY - mEnvelopeMargin);
    
    g.drawText("-1", 25,                              0, 20, 10, juce::Justification::centredTop);
    g.drawText("0" , (mBoundsEndPosX / 2) - 10,      0, 20, 10, juce::Justification::centredTop);
    g.drawText("1" , mBoundsEndPosX - 50,             0, 20, 10, juce::Justification::centredTop);
    
    g.drawText("-1", 0, 20,                              20, 10, juce::Justification::centredTop);
    g.drawText("0" ,  0, (mBoundsEndPosY / 2) - 8,      20, 10, juce::Justification::centredTop);
    g.drawText("1" ,  0, mBoundsEndPosY - 50,           20, 10, juce::Justification::centredTop);
    
    if (mCurve == nullptr)
        return;
    
    // ====== DEBUG DRAW =======
//    g.drawRect(0, 0, mBoundsEndPosX, mBoundsEndPosY);
//    g.drawRect(mEnvelopeStartX, mEnvelopeStartY, mEnvelopeEndX - mEnvelopeMargin, mEnvelopeEndY - mEnvelopeMargin);
    // ====== DEBUG END ========
    
    juce::Path p;
    Vector2 pos = mCurve->mPoints[0].GetDrawPosition(mEnvelopeSizeX, mEnvelopeSizeY);
    
    p.startNewSubPath((int)pos.x + mEnvelopeMargin, (int)pos.y + mEnvelopeMargin);

    for (int i = 0; i < (int)mCurve->mPoints.size(); ++i)
    {
        Vector2 pos = mCurve->mPoints[i].GetDrawPosition(mEnvelopeSizeX, mEnvelopeSizeY);
        
        juce::String x = juce::String(mCurve->mPoints[i].mPosition.x, 3);
        juce::String y = juce::String(mCurve->mPoints[i].mPosition.y, 3);
        
        if (i > 0)
        {
            p.lineTo((int)pos.x + mEnvelopeMargin, (int)pos.y + mEnvelopeMargin);
        }
        
        g.setColour(PINK);
        g.drawText("x: " + x + "  y: " + y, pos.x + mEnvelopeMargin + 20, pos.y + mEnvelopeMargin - 30, 200, 60, juce::Justification::centredLeft);
        
        g.setColour(TURQOISE);
        g.fillEllipse(pos.x + mEnvelopeMargin - POINT_HALF_SIZE,
                      pos.y + mEnvelopeMargin - POINT_HALF_SIZE,
                      POINT_SIZE, POINT_SIZE);
    }

    g.setColour(TURQOISE);
    g.strokePath(p, juce::PathStrokeType (LINE_WIDTH));
}

void Envelope::mouseDown (const juce::MouseEvent& event)
{
    for (int i = 0; i < (int)mCurve->mPoints.size(); ++i)
    {
        Vector2 pos = mCurve->mPoints[i].GetDrawPosition(mEnvelopeSizeX, mEnvelopeSizeY);
        if (abs(event.x - mEnvelopeMargin - pos.x) <= POINT_SIZE &&
            abs(event.y - mEnvelopeMargin - pos.y) <= POINT_SIZE)
        {
            mEditing = &mCurve->mPoints[i];
            mEditingIdx = i;
            break;
        }
    }
}

void Envelope::mouseDoubleClick(const juce::MouseEvent& event)
{
    for (int i = 0; i < (int)mCurve->mPoints.size(); ++i)
    {
        Vector2 pos = mCurve->mPoints[i].GetDrawPosition(mEnvelopeSizeX, mEnvelopeSizeY);
        if (abs(event.x - mEnvelopeMargin - pos.x) <= POINT_SIZE &&
            abs(event.y - mEnvelopeMargin - pos.y) <= POINT_SIZE)
        {
            mCurve->mPoints[i].Reset();
            notifyApvts(i);
            break;
        }
    }
    
    mCurve->mDirty = true;
    repaint();
    if (mSine != nullptr)
        mSine->repaint();
}

void Envelope::mouseDrag (const juce::MouseEvent& event)
{
    if(mEditing != nullptr)
    {
        int x = std::clamp(event.x - mEnvelopeMargin, 0, mEnvelopeSizeX);
        int y = std::clamp(event.y - mEnvelopeMargin, 0, mEnvelopeSizeY);
        
        mEditing->SetPositionFromMouse(x, y, mEnvelopeSizeX, mEnvelopeSizeY);
        notifyApvts(mEditingIdx);
        
        mCurve->mDirty = true;
        repaint();
        if (mSine != nullptr)
            mSine->repaint();
    }
}

void Envelope::mouseUp (const juce::MouseEvent& event)
{
    mEditing = nullptr;
    mEditingIdx = -1;
}

void Envelope::notifyApvts (int idx)
{
    if (mApvts == nullptr || mCurve == nullptr || idx < 0)
        return;

    auto set = [&](const juce::String& id, float val)
    {
        if (auto* param = mApvts->getParameter(id))
            param->setValueNotifyingHost(param->convertTo0to1(val));
    };

    switch (idx)
    {
        case 0: set("pt0y", mCurve->mPoints[0].mPosition.y); break;
        case 1: set("pt1x", mCurve->mPoints[1].mPosition.x); set("pt1y", mCurve->mPoints[1].mPosition.y); break;
        case 2: set("pt2x", mCurve->mPoints[2].mPosition.x); set("pt2y", mCurve->mPoints[2].mPosition.y); break;
        case 3: set("pt3x", mCurve->mPoints[3].mPosition.x); set("pt3y", mCurve->mPoints[3].mPosition.y); break;
        case 4: set("pt4y", mCurve->mPoints[4].mPosition.y); break;
        default: break;
    }
}
