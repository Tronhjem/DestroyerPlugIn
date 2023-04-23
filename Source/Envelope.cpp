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
    g.drawLine(EnvelopeMargin, BoundsEndPosY / 2, BoundsEndPosX - EnvelopeMargin, BoundsEndPosY / 2);
    g.drawLine(BoundsEndPosX / 2, EnvelopeMargin, BoundsEndPosX / 2, BoundsEndPosY - EnvelopeMargin);
    
    g.drawText("-1", 25,                        0, 20, 10, juce::Justification::centredTop);
    g.drawText("0", (BoundsEndPosX / 2) - 10,  0, 20, 10, juce::Justification::centredTop);
    g.drawText("1", BoundsEndPosX - 50,        0, 20, 10, juce::Justification::centredTop);
    
    g.drawText("-1", 0, 20,                        20, 10, juce::Justification::centredTop);
    g.drawText("0",  0, (BoundsEndPosY / 2) - 8, 20, 10, juce::Justification::centredTop);
    g.drawText("1",  0, BoundsEndPosY - 50      , 20, 10, juce::Justification::centredTop);
    
    if (curve == nullptr)
        return;
    
    // ====== DEBUG DRAW =======
//    g.drawRect(0, 0, BoundsEndPosX, BoundsEndPosY);
//    g.drawRect(EnvelopeStartX, EnvelopeStartY, EnvelopeEndX - EnvelopeMargin, EnvelopeEndY - EnvelopeMargin);
    // ====== DEBUG END ========
    
    juce::Path p;
    Vector2 pos = curve->points[0].GetDrawPosition(EnvelopeSizeX, EnvelopeSizeY);
    
    p.startNewSubPath((int)pos.x + EnvelopeMargin, (int)pos.y + EnvelopeMargin);

    for (int i = 0; i < curve->points.size(); ++i)
    {
        Vector2 pos = curve->points[i].GetDrawPosition(EnvelopeSizeX, EnvelopeSizeY);
        
        juce::String x = juce::String(curve->points[i].position.x, 3);
        juce::String y = juce::String(curve->points[i].position.y, 3);
        
        if (i > 0)
        {
            p.lineTo((int)pos.x + EnvelopeMargin, (int)pos.y + EnvelopeMargin);
        }
        
        g.setColour(PINK);
        g.drawText("x: " + x + "  y: " + y, pos.x + EnvelopeMargin + 20, pos.y + EnvelopeMargin - 30, 200, 60, juce::Justification::centredLeft);
        
        g.setColour(TURQOISE);
        g.fillEllipse(pos.x + EnvelopeMargin - POINT_HALF_SIZE,
                      pos.y + EnvelopeMargin - POINT_HALF_SIZE,
                      POINT_SIZE, POINT_SIZE);
        
    }

    g.setColour(TURQOISE);
    g.strokePath(p, juce::PathStrokeType (LINE_WIDTH));
}

void Envelope::mouseDown (const juce::MouseEvent& event)
{
    for (int i = 0; i < curve->points.size(); ++i)
    {
        Vector2 pos = curve->points[i].GetDrawPosition(EnvelopeSizeX, EnvelopeSizeY);
        if (abs(event.x - EnvelopeMargin - pos.x) <= POINT_SIZE &&
            abs(event.y - EnvelopeMargin - pos.y) <= POINT_SIZE)
        {
            editing = &curve->points[i];
            // opt out since we found the point.
            break;
        }
    }
}

void Envelope::mouseDoubleClick(const juce::MouseEvent& event)
{
    for (int i = 0; i < curve->points.size(); ++i)
    {
        Vector2 pos = curve->points[i].GetDrawPosition(EnvelopeSizeX, EnvelopeSizeY);
        if (abs(event.x - EnvelopeMargin - pos.x) <= POINT_SIZE &&
            abs(event.y - EnvelopeMargin - pos.y) <= POINT_SIZE)
        {
            curve->points[i].Reset();
            break;
        }
    }
    
    repaint();
    if (sine != nullptr)
        sine->repaint();
}

void Envelope::mouseDrag (const juce::MouseEvent& event)
{
    if(editing != nullptr)
    {
        int x = std::clamp(event.x - EnvelopeMargin, 0, EnvelopeSizeX);
        int y = std::clamp(event.y - EnvelopeMargin, 0, EnvelopeSizeY);
        
        editing->SetPositionFromMouse(x, y, EnvelopeSizeX, EnvelopeSizeY);
        
        repaint();
        if (sine != nullptr)
            sine->repaint();
    }
}

void Envelope::mouseUp (const juce::MouseEvent& event)
{
    editing = nullptr;
}
