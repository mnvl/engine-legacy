
#include "connection_handler.h"

namespace starmastersd
{

void ConnectionHandler::process(server::Connection *connection_ptr, boost::uint16_t id, util::Buffer &buf)
{
}

void ConnectionHandler::error(server::Connection *connection_ptr, boost::system::error_code const &error)
{
}

}