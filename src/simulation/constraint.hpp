#ifndef UCLOTH_CONSTRAINT_H
#define UCLOTH_CONSTRAINT_H

#include <umath.hpp>
#include "uclothcommon.hpp"

namespace ucloth
{ 
namespace simulation
{

    struct DistanceConstraint
    {
        Particle p1;
        Particle p2;

        umath::Real distance;
        umath::Real stiffness;
    };
    
} // simulation
} // ucloth

#endif