
#include <luabind/luabind.hpp>
#include "input.h"
#include "mouse.h"

void platform::Mouse::bind(lua_State *L)
{
	using namespace luabind;

	module_(L, "platform")
	[
		class_<Mouse, boost::shared_ptr<Mouse> >("Mouse")
		.def(constructor<boost::shared_ptr<Input> const &, bool, bool>())
		.def("update", &Mouse::update)
	];
}
