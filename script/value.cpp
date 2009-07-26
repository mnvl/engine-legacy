
#include <boost/variant/apply_visitor.hpp>
#include "table.h"
#include "value.h"

namespace script
{

namespace
{
struct push_visitor: boost::static_visitor<>
{
	lua_State *L_;

	push_visitor(lua_State *L):
		L_(L)
	{
	}

	void operator()(double const &d) const
	{
		lua_pushnumber(L_, d);
	}

	void operator()(std::string const &str) const
	{
		lua_pushstring(L_, str.c_str());
	}

	void operator()(lua_CFunction function) const
	{
		lua_pushcclosure(L_, function, 0);
	}

	void operator()(boost::shared_ptr<class table> const &table_ptr) const
	{
		table_ptr->push(L_);
	}
};
}

void pushvalue(lua_State *L, value_t const &value)
{
	boost::apply_visitor(push_visitor(L), value);
}

void getvalue(value_t &v, lua_State *L, int narg)
{
	switch (lua_type(L, narg))
	{
	case LUA_TNUMBER:
		v = lua_tonumber(L, narg);
		break;

	case LUA_TSTRING:
		v = lua_tostring(L, narg);
		break;

	case LUA_TUSERDATA:
		v = table::get(L, narg);
		break;
	}
}

}
