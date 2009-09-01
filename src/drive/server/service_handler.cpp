
#include <rpc/handler.h>
#include <drive/model/clock.h>
#include <drive/model/model.h>
#include <drive/proto/service.pb.h>
#include <util/logger.h>
#include "service_handler.h"

namespace drive
{

void ServiceHandler::handle_handshake(boost::shared_ptr<rpc::Connection> const &connection_ptr, rpc::Buffer &buf)
{
	proto::service::handshake message;
	message.ParseFromArray(buf.read_ptr(), buf.read_bytes_available());

	proto::service::handshake_reply reply;
	reply.set_is_supported(message.protocol_version() == 1);

	connection_ptr->write(reply);
}

void ServiceHandler::handle_time_sync(boost::shared_ptr<rpc::Connection> const &connection_ptr, rpc::Buffer &buf)
{
	proto::service::time_sync message;
	message.ParseFromArray(buf.read_ptr(), buf.read_bytes_available());

	proto::service::time_sync_reply reply;
	reply.set_server_time(model::Model::clock().millisecs());
	connection_ptr->write(reply);
}

void ServiceHandler::handle_logic_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, rpc::Buffer &buf)
{
	proto::service::logic_error message;
	message.ParseFromArray(buf.read_ptr(), buf.read_bytes_available());

	LOG_ERROR("logic error received: " << message.message());

	throw rpc::Handler::CloseConnectionException();
}

void ServiceHandler::handle_internal_error(boost::shared_ptr<rpc::Connection> const &connection_ptr, rpc::Buffer &buf)
{
	proto::service::internal_error message;
	message.ParseFromArray(buf.read_ptr(), buf.read_bytes_available());

	LOG_ERROR("internal error received: " << message.message());

	throw rpc::Handler::CloseConnectionException();
}

}