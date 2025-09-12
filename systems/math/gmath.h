#pragma once
#include <cstdlib>

// you might think this means "Game Math" since this is inside a game engine code
//
// WRONG!
// Gay Math.

namespace GMath
{

template<typename T>
inline int Sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

inline bool EqAbs(double a, double b, double eps = 0.000001)
{
    return std::abs(a - b) < eps;
}
};
