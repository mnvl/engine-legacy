
#include <luabind/luabind.hpp>
#include "input.h"

void platform::Input::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "platform")
	[
		class_<Input, boost::shared_ptr<Input> >("Input")
		.def(constructor<boost::shared_ptr<Window> const &>())
	];
}
