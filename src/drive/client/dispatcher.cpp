
#include <util/logger.h>
#include <drive/proto/id.pb.h>
#include <drive/proto/service.pb.h>
#include "global.h"
#include "dispatcher.h"

namespace drive
{
namespace client
{

Dispatcher::Dispatcher():
	state_(STATE_CONNECT),
	time_syncs_(0)
{
}

Dispatcher::~Dispatcher()
{
}

void Dispatcher::tick()
{
	boost::uint16_t id;
	rpc::BufferPtr buffer;

	bool processed = false;

	boost::tie(id, buffer) = Global::client().receive();

	if (state_ == STATE_CONNECT)
	{
		if (Global::client().is_connected())
		{
			state_ = STATE_HANDSHAKE;
		}

		processed = true;
	}

	if (state_ == STATE_HANDSHAKE)
	{
		LOG_DEBUG("sending handshake...");

		proto::service::handshake message;
		message.set_protocol_version(1);
		Global::client().send(proto::SERVICE_HANDSHAKE, message);

		state_ = STATE_HANDSHAKE_REPLY;
		processed = true;
	}

	if (state_ == STATE_HANDSHAKE_REPLY && id == proto::SERVICE_HANDSHAKE_REPLY)
	{
		proto::service::handshake_reply message;
		message.ParseFromArray(buffer->read_ptr(), buffer->read_bytes_available());

		LOG_DEBUG("handshake message received: is_supported = " << message.is_supported());

		if (message.is_supported())
		{
			state_ = STATE_TIME_SYNC;
			processed = true;
		}
	}

	if (state_ == STATE_TIME_SYNC)
	{
		LOG_DEBUG("sending time sync iteration " << time_syncs_ << "...");

		proto::service::time_sync message;
		Global::client().send(proto::SERVICE_TIME_SYNC, message);

		if (time_syncs_ < 5)
		{
			state_ = STATE_TIME_SYNC_REPLY;
			processed = true;
		}
	}

	if (state_ == STATE_TIME_SYNC_REPLY)
	{
		LOG_DEBUG("time sync reply received");

		time_syncs_++;

		state_ = STATE_TIME_SYNC;
		processed = true;
	}

	if (id && !processed)
	{
		throw ProtocolError();
	}
}

}
}
