//
//  Envelope.hpp
//  Destroyer_StandaloneGUI - App
//
//  Created by Christian Tronhjem on 22.04.23.
//

#ifndef Envelope_hpp
#define Envelope_hpp
#include <vector>
#include <JuceHeader.h>

static int MousePosX;
static int MousePosY;
    
constexpr int POINT_SIZE = 10;
constexpr float POINT_HALF_SIZE = (float)POINT_SIZE * 0.5f;

constexpr int LINE_WIDTH = 2;
constexpr float HALF_LINE_WIDTH = (float)LINE_WIDTH * 0.5f;

constexpr float POINT_MARGIN = POINT_HALF_SIZE + HALF_LINE_WIDTH;

enum PointRestrict
{
    None = 0,
    LockX = 1,
    LockY = 2,
    LockXY = 3
};

struct Vector2
{
    float x, y;
};
class SineWaveVisual;

class EnvelopePoint
{
public:
    EnvelopePoint(){}
    EnvelopePoint(Vector2 pos, PointRestrict restriction = PointRestrict::None) : position(pos), originalPosition(pos), restrictionType(restriction)
    {
        
    }
    
    Vector2 GetDrawPosition(int xMaxSize, int yMaxSize)
    {
        Vector2 pos;
        pos.x = (position.x + 1.f) * 0.5f * xMaxSize;
        pos.y = (position.y + 1.f) * 0.5f * yMaxSize;
        return pos;
    }
    
    void SetPositionFromMouse(int x, int y, int xMaxSize, int yMaxSize)
    {
        if (restrictionType == PointRestrict::LockXY)
            return;
        
        if (restrictionType != PointRestrict::LockX)
        {
            position.x = (float) x / (float) xMaxSize;
            position.x = (position.x * 2.f) - 1.f;
        }
        
        if (restrictionType != PointRestrict::LockY)
        {
            position.y = (float) y / (float) yMaxSize;
            position.y = (position.y * 2.f) - 1.f;
        }
    }
    
    void Reset()
    {
        position = originalPosition;
    }

    Vector2 position;
    Vector2 originalPosition;
    PointRestrict restrictionType = PointRestrict::None;
};

class TranslationCurve
{
public:
    TranslationCurve()
    {
        points.push_back(EnvelopePoint{ Vector2{-1.f, -1.f}, PointRestrict::LockX});
        points.push_back(EnvelopePoint{ Vector2{-0.5f, -0.5f}                           });
        points.push_back(EnvelopePoint{ Vector2{0.f, 0.f},   });
        points.push_back(EnvelopePoint{ Vector2{0.5f, 0.5f}                             });
        points.push_back(EnvelopePoint{ Vector2{1.f, 1.f},   PointRestrict::LockX});
    }
    
    std::vector<EnvelopePoint> points;
    
    float GetYValue(float x)
    {
        if (x > 1.f)
            x = 1.f;
            
        if (x < -1.f)
            x = -1.f;
        
        EnvelopePoint* startPoint = nullptr;
        EnvelopePoint* endPoint = nullptr;
        
        for (int i = 1; i < points.size(); ++i)
        {
            if(x >= points[i - 1].position.x &&
               x <= points[i].position.x)
            {
                startPoint = &points[i - 1];
                endPoint = &points[i];
                break;
            }
        }
        
        float value = 0.f;
        if (startPoint != nullptr && endPoint != nullptr)
        {
            // get the size and calculate how far the point is between the two.
            float size = endPoint->position.x - startPoint->position.x;
            float t = (x - startPoint->position.x) / size;
            value = lerp(startPoint->position.y, endPoint->position.y, t);
        }
        
        return value;
    }
    
    inline float lerp(float a, float b, float t)
    {
        return (1.f - t) * a + t * b;
    }
};

class Envelope : public juce::Component
{
public:
    Envelope(int x, int y, int width, int height, SineWaveVisual* s)
    {
        BoundsX = x;
        BoundsY = y;
        BoundsEndPosX = width;
        BoundsEndPosY = height;
        BoundsSizeX = width - x;
        BoundsSizeY = height - y;
        
        EnvelopeStartX = EnvelopeMargin;
        EnvelopeStartY = EnvelopeMargin;
        EnvelopeEndX = width - EnvelopeMargin;
        EnvelopeEndY = height - EnvelopeMargin;
        EnvelopeSizeX = EnvelopeEndX - EnvelopeMargin;
        EnvelopeSizeY = EnvelopeEndY - EnvelopeMargin;
        
        setBounds(x, y, width, height);
        
        sine = s;
    }
    
    TranslationCurve* curve;
    SineWaveVisual* sine;
    
private:
    void paint(juce::Graphics &g) override;
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;

    int BoundsX;
    int BoundsY;
    int BoundsEndPosX;
    int BoundsEndPosY;
    int BoundsSizeX;
    int BoundsSizeY;
    
    const int EnvelopeMargin = 40;
    int EnvelopeStartX;
    int EnvelopeStartY;
    int EnvelopeEndX;
    int EnvelopeEndY;
    int EnvelopeSizeX;
    int EnvelopeSizeY;

    EnvelopePoint* editing = nullptr;
};

#endif /* Envelope_hpp */
