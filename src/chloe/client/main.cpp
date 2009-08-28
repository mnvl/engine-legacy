
#include <windows.h>
#include <util/logger.h>
#include "global.h"

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
	try
	{
		util::Logger::instance().set_file("chloe.log");
		chloe::Global::instance().Start();
		chloe::Global::instance().Run();
		chloe::Global::instance().Stop();
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