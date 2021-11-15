#include "nlglobals.h"

#include <QVariant>
#include <QDebug>

#include "nldef.h"
#include "nlphysics.h"

static bool _engine_inited = false;

namespace NL
{
bool init_engine()
{
    if(_engine_inited)
        return true;

    qRegisterMetaType<NL::Physics::m>("NL::Physics::m");
    qRegisterMetaType<NL::Physics::a>("NL::Physics::a");
    qRegisterMetaType<NL::Physics::g>("NL::Physics::g");
    qRegisterMetaType<NL::Physics::G>("NL::Physics::G");
    qRegisterMetaType<NL::Physics::v>("NL::Physics::v");
    qRegisterMetaType<NL::Physics::F>("NL::Physics::F");
    qRegisterMetaType<NL::Physics::t>("NL::Physics::t");
    qRegisterMetaType<NL::Physics::d>("NL::Physics::d");

    _engine_inited = true;
    return _engine_inited;
}

bool engine_is_inited()
{
    return _engine_inited;
}
}
