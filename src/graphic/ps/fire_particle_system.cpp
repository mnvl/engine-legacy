
#include <boost/shared_ptr.hpp>
#include <luabind/luabind.hpp>
#include "fire_particle_system.h"

namespace graphic
{
namespace ps
{

void bind_FireParticleSystem(lua_State *L)
{
	using namespace luabind;

    module_(L, "graphic")
    [
        class_<FireParticleSystem, boost::shared_ptr<FireParticleSystem> >("FireParticleSystem")
    ];
}

}
}
