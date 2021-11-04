#include "nlphysics.h"

namespace NL
{
    namespace Physics
    {
        const float EARTH_G = 9.8f;

        v speed(t t0, a a0, v v0)
        {
            return a0 * t0 + v0;
        }

        d distance(t t0, v v0, a a0)
        {
            return v0 * t0 + a0 * (t0 * t0) / 2;
        }

        a acceleration(F F0, m m0)
        {
            return F0 / m0;
        }

        G gravity_force(m m0, g g0)
        {
            return m0 * g0;
        }

        v gravity_speed(t t0, v v0, g g0)
        {
            return speed(t0, g0, v0);
        }

        G gravity_distance(t t0, v v0, g g0)
        {
            return distance(t0, v0, g0);
        }
    }
}
