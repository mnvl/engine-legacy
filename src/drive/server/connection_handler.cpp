
#include <drive/proto/id.pb.h>
#include "service_handler.h"
#include "connection_handler.h"

namespace drive
{

void ConnectionHandler::on_connect(boost::shared_ptr<rpc::Connection> const &connection_ptr)
{
}

void ConnectionHandler::on_read(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::uint16_t id, rpc::Buffer &buf)
{
	switch (id)
	{
	case proto::SERVICE_HANDSHAKE:
		ServiceHandler::handle_handshake(connection_ptr, buf);
		break;

	case proto::SERVICE_TIME_SYNC:
		ServiceHandler::handle_time_sync(connection_ptr, buf);
		break;

	case proto::SERVICE_LOGIC_ERROR:
		ServiceHandler::handle_logic_error(connection_ptr, buf);
		break;

	case proto::SERVICE_INTERNAL_ERROR:
		ServiceHandler::handle_internal_error(connection_ptr, buf);
		break;

	default:
		throw CloseConnectionException();
	}
}

void ConnectionHandler::on_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, boost::system::error_code const &error)
{
}

}
