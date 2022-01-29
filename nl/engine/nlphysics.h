#ifndef _KARIN_NLPHYSICS_H
#define _KARIN_NLPHYSICS_H

#include <QtGlobal>

#include "common/nlinc.h"

namespace NL
{
    namespace Physics
    {
        typedef float m;
        typedef float a;
        typedef float g;
        typedef float G;
        typedef float v;
        typedef float F;
        typedef float t;
        typedef float d;
        extern const NLLIB_EXPORT float EARTH_G;

        NLLIB_EXPORT v speed(t t0, a a0 = 0, v v0 = 0); // v = at + v0
        NLLIB_EXPORT d distance(t t0, v v0 = 0, a a0 = 0); // x = v0t + at^2 / 2;
        NLLIB_EXPORT a acceleration(F F0, m m0); // F = ma

        NLLIB_EXPORT G gravity_force(m m0, g g0 = EARTH_G); // G = mg
        NLLIB_EXPORT v gravity_speed(t t0, v v0 = 0, g g0 = EARTH_G); // v = gt + v0
        NLLIB_EXPORT d gravity_distance(t t0, v v0 = 0, g g0 = EARTH_G);
    }
}

namespace NL
{
    namespace Physics
    {
        inline v speed(t t0, a a0, v v0)
        {
            return a0 * t0 + v0;
        }

        inline d distance(t t0, v v0, a a0)
        {
            return v0 * t0 + a0 * (t0 * t0) / 2;
        }

        inline a acceleration(F F0, m m0)
        {
            return F0 / m0;
        }

        inline G gravity_force(m m0, g g0)
        {
            return m0 * g0;
        }

        inline v gravity_speed(t t0, v v0, g g0)
        {
            return speed(t0, g0, v0);
        }

        inline d gravity_distance(t t0, v v0, g g0)
        {
            return distance(t0, v0, g0);
        }
    }
}

#endif // _KARIN_NLPHYSICS_H
