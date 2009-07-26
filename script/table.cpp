
#include "table.h"

namespace script
{

char const table::METATABLE_NAME[] = "__script_table";

table::table(descr const *table_descr):
	descr_(table_descr),
	fields_(table_descr ? table_descr->nfields() : 0)
{

}

table::~table()
{
}

void table::bind(lua_State *L)
{
	luaL_newmetatable(L, METATABLE_NAME);

	lua_pushstring(L, "__gc");
	lua_pushcclosure(L, &table::lua_gc, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__index");
	lua_pushcclosure(L, &table::lua_index, 0);
	lua_rawset(L, -3);

	lua_pushstring(L, "__newindex");
	lua_pushcclosure(L, &table::lua_newindex, 0);
	lua_rawset(L, -3);

	lua_pop(L, 1);
}

void table::push(lua_State *L)
{
	void *userdata = lua_newuserdata(L, sizeof(boost::shared_ptr<table>));
	new (userdata) boost::shared_ptr<table>(shared_from_this());

	lua_pushstring(L, METATABLE_NAME);
	lua_rawget(L, LUA_REGISTRYINDEX);

	lua_setmetatable(L, -2);
}

boost::shared_ptr<table> &table::get(lua_State *L, int narg)
{
	boost::shared_ptr<table> *table_ptr = (boost::shared_ptr<table> *) luaL_checkudata(L, narg, METATABLE_NAME);
	if (table_ptr == 0) throw std::runtime_error("can't convert argument to table pointer");
	return *table_ptr;
}

int table::lua_gc(lua_State *L)
{
	boost::shared_ptr<table> &table_ptr = get(L, 0);
	table_ptr.~shared_ptr<table>();
}

int table::lua_index(lua_State *L)
{
	boost::shared_ptr<table> &table_ptr = get(L, 1);

	switch (lua_type(L, 2))
	{
	case LUA_TNUMBER:
		pushvalue(L, table_ptr->at((size_t) lua_tonumber(L, 2)));
		break;

	case LUA_TSTRING:
		pushvalue(L, table_ptr->at(lua_tostring(L, 2)));
		break;

	default:
		lua_pushstring(L, "only number or string indexing of table supported");
		lua_error(L);
	}
}

int table::lua_newindex(lua_State *L)
{
	boost::shared_ptr<table> &table_ptr = get(L, 0);
}

}
