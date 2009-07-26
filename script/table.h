#pragma once

#include <vector>
#include <lua.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "value.h"
#include "descr.h"

namespace script
{

class table: public boost::enable_shared_from_this<table>
{
public:
	typedef std::vector<value_t> fields_t;

	table(descr const *table_descr);
	~table();

	void resize_fields(size_t size)
	{
		fields_.resize(size);
	}

	template <class T>
	void set(size_t index, T const &value)
	{
		at(index) = value_t(value);
	}

	template <class T>
	T get(size_t index) const
	{
		return boost::get<T>(at(index));
	}

	template <class T>
	void set(std::string const &name, T const &value)
	{
		at(name) = value_t(value);
	}

	template <class T>
	T get(std::string const &name) const
	{
		return boost::get<T>(at(name));
	}

	value_t &at(size_t index)
	{
		return fields_.at(index);
	}

	value_t const &at(size_t index) const
	{
		return fields_.at(index);
	}

	value_t &at(std::string const &name)
	{
		return at(descr_->index_by_name(name));
	}

	value_t const &at(std::string const &name) const
	{
		return at(descr_->index_by_name(name));
	}

	static void bind(lua_State *L);
	void push(lua_State *L); // pushes shared ptr to stack
	static boost::shared_ptr<table> &get(lua_State *L, int narg);

private:
	static char const METATABLE_NAME[];

	descr const *descr_;
	fields_t fields_;

	static int lua_gc(lua_State *L);
	static int lua_index(lua_State *L);
	static int lua_newindex(lua_State *L);
};

}
