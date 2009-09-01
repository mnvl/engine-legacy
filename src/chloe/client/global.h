#pragma once

#include <boost/scoped_ptr.hpp>
#include <platform/window.h>
#include <platform/input.h>
#include <platform/keyboard.h>
#include <platform/mouse.h>
#include <graphic/renderer.h>
#include <rpc/client_service.h>
#include "connection_handler.h"

namespace drive
{

class Global
{
public:
	Global();
	~Global();

	void Start();
	void Run();
	void Stop();

	static Global &instance();

	static rpc::ClientService &service()
	{
		return instance.service_;
	}

private:
	boost::scoped_ptr<platform::Window> window_;
	boost::scoped_ptr<platform::Input> input_;
	boost::scoped_ptr<platform::Keyboard> keyboard_;
	boost::scoped_ptr<platform::Mouse> mouse_;
	boost::scoped_ptr<graphic::Renderer> renderer_;
	boost::scoped_ptr<ConnectionHandler> connection_handler_;
	boost::scoped_ptr<rpc::ClientService> service_;
};

}