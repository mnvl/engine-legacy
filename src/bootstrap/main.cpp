//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <crtdbg.h>
#include <windows.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <boost/filesystem.hpp>
#include <luabind/luabind.hpp>
#include <luabind/detail/pcall.hpp>
#include <log4cplus/configurator.h>
#include <graphic/bind.h>
#include <platform/timer.h>
#include <platform/window.h>
#include <platform/input.h>
#include <platform/keyboard.h>
#include <platform/mouse.h>
#include <math/bind.h>
#include <spatial/bind.h>
#include <particle_system/bind.h>
#include <util/logger.h>
#include <util/archive.h>

extern "C"
{
	#include <lualib.h>

	LUALIB_API int luaopen_bit(lua_State *L);
}

class Main
{
public:
    Main():
		L_(lua_open())
	{
		xercesc::XMLPlatformUtils::Initialize();

		luaL_openlibs(L_);
		luabind::open(L_);

		graphic::bind(L_);

		platform::Timer::bind(L_);
		platform::Window::bind(L_);

		math::bind(L_);
		spatial::bind(L_);

		particle_system::bind(L_);

		util::bind_logger(L_);
		util::ArchiveReader::bind(L_);
		util::ArchiveWriter::bind(L_);

		platform::Input::bind(L_);
		platform::Keyboard::bind(L_);
		platform::Mouse::bind(L_);
	}

    ~Main()
	{
		lua_close(L_);

        xercesc::XMLPlatformUtils::Terminate();
	}

	void run(char const *filename)
	{
		lua_getglobal(L_, "debug");
		lua_getfield(L_, -1, "traceback");
		lua_remove(L_, -2);

		int result = luaL_loadfile(L_, filename);

		if (result == 0) result = lua_pcall(L_, 0, 0, -2);

		switch (result)
		{
		case 0:
			// success
			break;

		case LUA_ERRSYNTAX:
			throw std::runtime_error("syntax eror");

		case LUA_ERRRUN:
			throw std::runtime_error(lua_tostring(L_, -1));

		case LUA_ERRFILE:
			throw std::runtime_error("file not found");

		default:
			throw std::runtime_error("unknown error");
		}
	}

private:
    lua_State *L_;
	boost::shared_ptr<util::ArchiveReader> archive_reader_;
};

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
    try
    {
		log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
		Main().run(lpstrCmdLine);
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
