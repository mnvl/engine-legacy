
#include "global.h"

namespace starmastersd
{

Global::Global()
{
	connection_handler_.reset(new ConnectionHandler());
	service_.reset(new server::Service(connection_handler_.get()));
}

Global::~Global()
{
}

void Global::Start()
{
	service_->start();
}

void Global::Stop()
{
	service_->stop();
}

void Global::Wait()
{
	serice_->wait();
}

Global &Global::instance()
{
	Global global;
	return global;
}

}