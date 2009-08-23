
#include "connection_handler.h"

namespace starmastersd
{

void ConnectionHandler::process(rpc::Connection *connection_ptr, boost::uint16_t id, rpc::Buffer &buf)
{
}

void ConnectionHandler::error(rpc::Connection *connection_ptr, boost::system::error_code const &error)
{
}

}
