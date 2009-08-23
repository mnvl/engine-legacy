#pragma once

#include <iosfwd>
#include <boost/cstdint.hpp>
#include <boost/system/error_code.hpp>
#include <util/buffer.h>

namespace server
{

class Connection;

class Handler
{
public:
	class CloseConnection {};

	virtual void process(Connection *connection_ptr, boost::uint16_t id, util::Buffer &buf) = 0;
	virtual void error(Connection *connection_ptr, boost::system::error_code const &error) = 0;
};

}
