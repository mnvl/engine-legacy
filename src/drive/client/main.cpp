
#include <windows.h>
#include <util/logger.h>
#include "global.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
	try
	{
		util::Logger::instance().set_file("drive.log");
		drive::client::Global::instance().start();
		drive::client::Global::connection_handler().connect("localhost", "17171");
		drive::client::Global::instance().run();
		drive::client::Global::instance().stop();
		drive::client::Global::connection_handler().stop();
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