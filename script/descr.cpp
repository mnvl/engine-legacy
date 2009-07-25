
#include "descr.h"

namespace script
{

descr::descr(std::string const &name):
	name_(name)
{
}

descr::~descr()
{
}

size_t descr::add_field(std::string const &name)
{
	return name_index_.insert(std::make_pair(name, nfields())).first->second;
}

}
