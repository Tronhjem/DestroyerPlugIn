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
    EnvelopePoint(Vector2 pos, PointRestrict restriction = PointRestrict::None) : mPosition(pos), mOriginalPosition(pos), mRestrictionType(restriction)
    {
        
    }
    
    Vector2 GetDrawPosition(int xMaxSize, int yMaxSize) const
    {
        Vector2 pos;
        pos.x = (mPosition.x + 1.f) * 0.5f * xMaxSize;
        pos.y = (mPosition.y + 1.f) * 0.5f * yMaxSize;
        return pos;
    }
    
    void SetPositionFromMouse(int x, int y, int xMaxSize, int yMaxSize)
    {
        if (mRestrictionType == PointRestrict::LockXY)
            return;
        
        if (mRestrictionType != PointRestrict::LockX)
        {
            mPosition.x = (float) x / (float) xMaxSize;
            mPosition.x = (mPosition.x * 2.f) - 1.f;
        }
        
        if (mRestrictionType != PointRestrict::LockY)
        {
            mPosition.y = (float) y / (float) yMaxSize;
            mPosition.y = (mPosition.y * 2.f) - 1.f;
        }
    }
    
    void Reset()
    {
        mPosition = mOriginalPosition;
    }

    Vector2 mPosition;
    Vector2 mOriginalPosition;
    PointRestrict mRestrictionType = PointRestrict::None;
};

class TranslationCurve
{
public:
    TranslationCurve()
    {
        mPoints.push_back(EnvelopePoint{ Vector2{-1.f, -1.f}, PointRestrict::LockX});
        mPoints.push_back(EnvelopePoint{ Vector2{-0.5f, -0.5f}                    });
        mPoints.push_back(EnvelopePoint{ Vector2{0.f, 0.f},                       });
        mPoints.push_back(EnvelopePoint{ Vector2{0.5f, 0.5f}                      });
        mPoints.push_back(EnvelopePoint{ Vector2{1.f, 1.f},   PointRestrict::LockX});
    }
    
    std::vector<EnvelopePoint> mPoints;
    
    float GetYValue(float x) const
    {
        if (x > 1.f)
            x = 1.f;
            
        if (x < -1.f)
            x = -1.f;
        
        const EnvelopePoint* startPoint = nullptr;
        const EnvelopePoint* endPoint = nullptr;
        
        for (int i = 1; i < (int)mPoints.size(); ++i)
        {
            if(x >= mPoints[i - 1].mPosition.x &&
               x <= mPoints[i].mPosition.x)
            {
                startPoint = &mPoints[i - 1];
                endPoint = &mPoints[i];
                break;
            }
        }
        
        float value = 0.f;
        if (startPoint != nullptr && endPoint != nullptr)
        {
            // get the size and calculate how far the point is between the two.
            float size = endPoint->mPosition.x - startPoint->mPosition.x;
            float t = (x - startPoint->mPosition.x) / size;
            value = lerp(startPoint->mPosition.y, endPoint->mPosition.y, t);
        }
        
        return value;
    }
    
    static constexpr float lerp(float a, float b, float t) noexcept
    {
        return (1.f - t) * a + t * b;
    }
    
    bool mDirty = true;
};

class Envelope : public juce::Component
{
public:
    Envelope(int x, int y, int width, int height, SineWaveVisual* s)
    {
        mBoundsX = x;
        mBoundsY = y;
        mBoundsEndPosX = width;
        mBoundsEndPosY = height;
        mBoundsSizeX = width - x;
        mBoundsSizeY = height - y;
        
        mEnvelopeStartX = mEnvelopeMargin;
        mEnvelopeStartY = mEnvelopeMargin;
        mEnvelopeEndX = width - mEnvelopeMargin;
        mEnvelopeEndY = height - mEnvelopeMargin;
        mEnvelopeSizeX = mEnvelopeEndX - mEnvelopeMargin;
        mEnvelopeSizeY = mEnvelopeEndY - mEnvelopeMargin;
        
        setBounds(x, y, width, height);
        
        mSine = s;
    }
    
    TranslationCurve* mCurve;
    SineWaveVisual* mSine;
    juce::AudioProcessorValueTreeState* mApvts = nullptr;
    
private:
    void paint(juce::Graphics &g) override;
    void mouseDown (const juce::MouseEvent& event) override;
    void mouseDoubleClick(const juce::MouseEvent& event) override;
    void mouseDrag (const juce::MouseEvent& event) override;
    void mouseUp (const juce::MouseEvent& event) override;

    void notifyApvts (int pointIdx);

    int mBoundsX;
    int mBoundsY;
    int mBoundsEndPosX;
    int mBoundsEndPosY;
    int mBoundsSizeX;
    int mBoundsSizeY;
    
    const int mEnvelopeMargin = 40;
    int mEnvelopeStartX;
    int mEnvelopeStartY;
    int mEnvelopeEndX;
    int mEnvelopeEndY;
    int mEnvelopeSizeX;
    int mEnvelopeSizeY;

    EnvelopePoint* mEditing = nullptr;
    int mEditingIdx = -1;
};

#endif /* Envelope_hpp */
