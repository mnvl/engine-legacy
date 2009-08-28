#pragma once

#include <boost/scoped_ptr.hpp>
#include <platform/window.h>
#include <platform/input.h>
#include <platform/keyboard.h>
#include <platform/mouse.h>
#include <graphic/renderer.h>

namespace chloe
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

private:
	boost::scoped_ptr<platform::Window> window_;
	boost::scoped_ptr<platform::Input> input_;
	boost::scoped_ptr<platform::Keyboard> keyboard_;
	boost::scoped_ptr<platform::Mouse> mouse_;
	boost::scoped_ptr<graphic::Renderer> renderer_;
};

}