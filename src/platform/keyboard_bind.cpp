
#include <luabind/luabind.hpp>
#include "input.h"
#include "keyboard.h"

void platform::Keyboard::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "platform")
	[
		class_<Keyboard, boost::shared_ptr<Keyboard> >("Keyboard")
		.def(constructor<boost::shared_ptr<Input> const &>())
	];
}
