
#include "global.h"

namespace drive
{
namespace client
{

Global::Global()
{
	window_.reset(new platform::Window("drive"));
	input_.reset(new platform::Input(window_.get()));
	keyboard_.reset(new platform::Keyboard(input_.get()));
	mouse_.reset(new platform::Mouse(input_.get()));
	renderer_.reset(new graphic::Renderer(window_.get()));
	connection_.reset(new Connection());
}

Global::~Global()
{
}

void Global::start()
{
}

void Global::run()
{
	while (window_->run_message_loop())
	{
		renderer_->begin_frame();

		renderer_->finish_frame();
	}
}

void Global::stop()
{
}

Global &Global::instance()
{
	static Global global;
	return global;
}

}
}
