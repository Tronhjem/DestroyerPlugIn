//
//  MoogFilter.cpp
//  Destroyer - VST3
//
//  Created by Christian Tronhjem on 23.04.23.
//

#include "MoogFilter.hpp"
#include <algorithm>
#include <cmath>

double MoogFilter::Process(double in, double freq, double res)
{
    res = std::clamp(res, 0.0, 1.0);

    const double f = 2.0 * freq * mSampleRate;
    const double k = 3.6 * f - 1.6 * f * f - 1.0;
    const double p = (k + 1.0) * 0.5;
    const double scale = std::exp((1.0 - p) * 1.386249);
    const double r = res * scale;
    const double x = in - r * mY4;

    mY1 = x   * p + mOldX  * p - k * mY1;
    mY2 = mY1 * p + mOldY1 * p - k * mY2;
    mY3 = mY2 * p + mOldY2 * p - k * mY3;
    mY4 = mY3 * p + mOldY3 * p - k * mY4;

    mY4 = mY4 - mY4 * mY4 * mY4 * BANDLIMIT;

    mOldX  = x;
    mOldY1 = mY1;
    mOldY2 = mY2;
    mOldY3 = mY3;

    return mY4;
};
