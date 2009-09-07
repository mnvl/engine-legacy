#pragma once

#include <exception>
#include <boost/noncopyable.hpp>

namespace drive
{
namespace client
{

class Dispatcher: public boost::noncopyable
{
public:
	class ProtocolError: public std::exception {
	public:
		char const *what() const {
			return "protocol_error";
		}
	};

	Dispatcher();
	~Dispatcher();

	void tick();

private:
	enum state_t
	{
		STATE_CONNECT = 1,
		STATE_HANDSHAKE,
		STATE_HANDSHAKE_REPLY,
		STATE_TIME_SYNC,
		STATE_TIME_SYNC_REPLY
	};

	state_t state_;

	int time_syncs_;

	void connect_tick();
	void handshake_tick();
	void time_sync_tick();
};

}
}
