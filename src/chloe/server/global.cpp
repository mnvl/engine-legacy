
#include "global.h"

namespace chloe
{

Global::Global()
{
	connection_handler_.reset(new ConnectionHandler());
	service_.reset(new rpc::ServerService(connection_handler_.get()));
}

Global::~Global()
{
}

void Global::Start()
{
	service_->start("localhost", "17171", 16);
}

void Global::Stop()
{
	service_->stop();
}

void Global::Wait()
{
	service_->wait();
}

Global &Global::instance()
{
	static Global global;
	return global;
}

}
