//
//  MoogFilter.cpp
//  Destroyer - VST3
//
//  Created by Christian Tronhjem on 23.04.23.
//

#include "MoogFilter.hpp"
double MoogFilter::Process(double in, double freq, double res)
{
    if (res > 1.f)
        res = 1.f;
    if (res < 0.f)
        res = 0.f;

    double f = 2.0 * freq * SAMPLERATE;

    //k = 3.6*f - 1.6*f*f -1; //(Empirical tunning)
    
    double k = 3.6 * f - 1.6 * f * f - 1.0; //(Empirical tunning)

    double p = (k + 1.0) * 0.5;
    double scale = pow(MoogFilter::e, (1.0 - p) * 1.386249);
    double r = res * scale;

    double x = in - r * y4;

    //Four cascaded onepole filters (bilinear transform)
    y1 = x * p + oldx * p - k * y1;
    y2 = y1 * p + oldy1 * p - k * y2;
    y3 = y2 * p + oldy2 * p - k * y3;
    y4 = y3 * p + oldy3 * p - k * y4;

    //Clipper band limited sigmoid
    y4 = y4 - pow(y4, 3.0) * BANDLIMIT;

    oldx = x;
    oldy1 = y1;
    oldy2 = y2;
    oldy3 = y3;

    return y4;
};
