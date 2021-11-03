#ifndef _KARIN_NLFUNCS_H
#define _KARIN_NLFUNCS_H

namespace NL
{
template <class T> T clamp(T t, T min, T max);
}

template <class T> T NL::clamp(T t, T min, T max)
{
    return qMax(min, qMin(t, max));
}

#endif // _KARIN_NLFUNCS_H
