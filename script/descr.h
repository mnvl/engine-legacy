#pragma once

#include <boost/unordered_map.hpp>

namespace script
{

class descr
{
public:
	descr(std::string const &name);
	~descr();

	size_t add_field(std::string const &name);

	size_t nfields() const
	{
		return name_index_.size();
	}

	size_t index_by_name(std::string const &name) const
	{
		name_index_t::const_iterator it = name_index_.find(name);
		if (it == name_index_.end()) throw std::runtime_error("unknown field " + name + " of table descr " + name_);
		return it->second;
	}

	std::string const &name() const
	{
		return name_;
	}

private:
	typedef boost::unordered_map<std::string, size_t> name_index_t;

	std::string name_;
	name_index_t name_index_;
};

}
