#pragma once

#include <lua.hpp>
#include <boost/variant.hpp>

namespace script
{

typedef boost::variant<double, std::string, lua_CFunction, boost::shared_ptr<class table> > value_t;

void pushvalue(lua_State *L, value_t const &value);
void getvalue(value_t &v, lua_State *L, int narg);

}
