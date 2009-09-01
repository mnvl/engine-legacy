
#include <windows.h>
#include <util/logger.h>
#include "global.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
	try
	{
		util::Logger::instance().set_file("drive.log");
		drive::Global::instance().Start();
		drive::Global::instance().Run();
		drive::Global::instance().Stop();
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