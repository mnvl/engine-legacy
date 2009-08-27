
#include <util/logger.h>
#include "global.h"

int main(int argc, char *argv[])
{
	try
	{
		util::Logger::instance().set_stderr();

		starmastersd::Global::instance().Start();

		starmastersd::Global::instance().Wait();
	}
	catch (std::exception const &error)
	{
		LOG_ERROR("exception caught: " << error.what());
		return 1;
	}
	catch (...)
	{
		LOG_ERROR("unknown exception caught");
		return 1;
	}

	return 0;
}