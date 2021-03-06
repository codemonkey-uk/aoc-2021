#ifndef GEOMETRY_CONST_H_INCLUDED
#define GEOMETRY_CONST_H_INCLUDED

#include <cmath>

// geometry_constants.h
// (c) T.Frogley 2003 
// defines geometric constants, for types #include specfic headers

namespace Geometry
{
    const double phi = (1 + sqrt(5.0))/2;
    const double pi  = atan(1.0) * 4;
    const double tau = 2*pi;
}

#endif//GEOMETRY_CONST_H_INCLUDED