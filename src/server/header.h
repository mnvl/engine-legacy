#pragma once

#include <boost/cstdint.hpp>

namespace server
{

#pragma pack(push, 1)
struct header
{
	boost::uint16_t id;
	boost::uint16_t len;
};
#pragma pack(pop)

}
