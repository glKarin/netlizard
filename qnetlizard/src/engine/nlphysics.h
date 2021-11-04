#ifndef _KARIN_NLPHYSICS_H
#define _KARIN_NLPHYSICS_H

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
        extern const float EARTH_G;

        v speed(t t0, a a0 = 0, v v0 = 0); // v = at + v0
        d distance(t t0, v v0 = 0, a a0 = 0); // x = v0t + at^2 / 2;
        a acceleration(F F0, m m0); // F = ma

        G gravity_force(m m0, g g0 = EARTH_G); // G = mg
        v gravity_speed(t t0, v v0 = 0, g g0 = EARTH_G); // v = gt + v0
        G gravity_distance(t t0, v v0 = 0, g g0 = EARTH_G);
    }
}

#endif // _KARIN_NLPHYSICS_H
