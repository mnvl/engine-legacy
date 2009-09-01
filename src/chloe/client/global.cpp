
#include "global.h"

namespace drive
{

Global::Global()
{
	window_.reset(new platform::Window("starmasters"));
	input_.reset(new platform::Input(window_.get()));
	keyboard_.reset(new platform::Keyboard(input_.get()));
	mouse_.reset(new platform::Mouse(input_.get()));
	renderer_.reset(new graphic::Renderer(window_.get()));
	connection_handler_.reset(new ConnectionHandler());
	client_service_.reset(new rpc::ClientService(connection_handler_.get()));
}

Global::~Global()
{
}

void Global::Start()
{
}

void Global::Run()
{
	while (window_->run_message_loop())
	{
		renderer_->begin_frame();

		renderer_->finish_frame();
	}
}

void Global::Stop()
{
}

Global &Global::instance()
{
	static Global global;
	return global;
}

}