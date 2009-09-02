#pragma once

#include <boost/scoped_ptr.hpp>
#include <platform/window.h>
#include <platform/input.h>
#include <platform/keyboard.h>
#include <platform/mouse.h>
#include <graphic/renderer.h>
#include "connection_handler.h"

namespace drive
{
namespace client
{

class Global
{
public:
	Global();
	~Global();

	void start();
	void run();
	void stop();

	static Global &instance();

	static ConnectionHandler &connection_handler()
	{
		return *instance().connection_handler_.get();
	}

private:
	boost::scoped_ptr<platform::Window> window_;
	boost::scoped_ptr<platform::Input> input_;
	boost::scoped_ptr<platform::Keyboard> keyboard_;
	boost::scoped_ptr<platform::Mouse> mouse_;
	boost::scoped_ptr<graphic::Renderer> renderer_;
	boost::scoped_ptr<ConnectionHandler> connection_handler_;
};

}
}