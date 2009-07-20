#pragma once

#include <vector>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <luabind/lua_include.hpp>

namespace script
{

class table: public boost::enable_shared_from_this<table>
{
public:
	typedef boost::shared_ptr<table> table_ptr_t;
	typedef boost::variant<double, std::string, lua_CFunction, table_ptr_t> value_t;
	typedef std::vector<value_t> fields_t;

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

	value_t &at(size_t index)
	{
		return fields_.at(index);
	}

	value_t const &at(size_t index) const
	{
		return fields_.at(index);
	}

private:
	fields_t fields_;
};

}
