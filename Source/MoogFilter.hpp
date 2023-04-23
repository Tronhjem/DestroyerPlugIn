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
    double oldx     = 0.0;
    double oldy1    = 0.0;
    double oldy2    = 0.0;
    double oldy3    = 0.0;

    double y1       = 0.0;
    double y2       = 0.0;
    double y3       = 0.0;
    double y4       = 0.0;
    
    static constexpr double SAMPLERATE = 1.0 / 44100.0;
    static constexpr double BANDLIMIT = 1.0 / 6.0;
    static constexpr double e = 2.71828182845904523536028747135266249;

public:
    MoogFilter() {};
    double Process(double in, double freq, double res);
};
#endif /* MoogFilter_hpp */
