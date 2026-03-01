//
//  MoogFilter.hpp
//  Destroyer - VST3
//
//  Created by Christian Tronhjem on 23.04.23.
//

#ifndef MoogFilter_hpp
#define MoogFilter_hpp


#include <cmath>

class MoogFilter
{
private:
    double mOldX  = 0.0;
    double mOldY1 = 0.0;
    double mOldY2 = 0.0;
    double mOldY3 = 0.0;

    double mY1 = 0.0;
    double mY2 = 0.0;
    double mY3 = 0.0;
    double mY4 = 0.0;
    
    double mSampleRate = 1.0 / 44100.0;
    static constexpr double BANDLIMIT = 1.0 / 6.0;

public:
    MoogFilter() {};
    void setSampleRate(double sr) noexcept { mSampleRate = 1.0 / sr; }
    double Process(double in, double freq, double res);
};
#endif /* MoogFilter_hpp */
