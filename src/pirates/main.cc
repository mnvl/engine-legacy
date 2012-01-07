//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <windows.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <boost/filesystem.hpp>
#include <luabind/luabind.hpp>
#include <luabind/detail/pcall.hpp>
#include <log4cplus/configurator.h>
#include <graphic/bind.h>
#include <graphic/d3d9_renderer.h>
#include <math/bind.h>
#include <spatial/bind.h>
#include <util/logger.h>
#include <util/archive.h>
#include <util/bind_boost.h>
#include <util/bind_sdl.h>
#include <util/bind_stl.h>
#include "heightfield.h"

extern "C"
{
	#include <lualib.h>
}

namespace
{
graphic::RendererPtr create_renderer_for_editor(int w, int h, luabind::object const &window_handle) {
	return graphic::RendererPtr(
		new graphic::D3D9Renderer(w, h, false, (HWND) lua_touserdata(window_handle.interpreter(), -1)));
}
}

class Main {
public:
    Main():
		L_(lua_open())
	{
		xercesc::XMLPlatformUtils::Initialize();

		luaL_openlibs(L_);
		luabind::open(L_);

		graphic::bind(L_);

		math::bind(L_);
		spatial::bind(L_);

		util::bind_logger(L_);
		util::ArchiveReader::bind(L_);
		util::ArchiveWriter::bind(L_);

		util::bind_boost(L_);
		util::bind_SDL(L_);
		util::bind_stl(L_);

		pirates::Heightfield::bind(L_);

		if (boost::filesystem::exists(boost::filesystem::path("pack0.dat")))
		{
			archive_reader_.reset(new util::ArchiveReader());
			archive_reader_->open("pack0.dat");
			luabind::globals(L_)["SCRIPTS_ARCHIVE_READER"] = archive_reader_;
		}

		luabind::module_(L_, "graphic")
		[
			luabind::def("create_renderer_for_editor", &create_renderer_for_editor)
		];
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

		int result;

		if (archive_reader_)
		{
			std::vector<char> buffer;
			archive_reader_->read_data(filename, buffer);
			result = luaL_loadbuffer(L_, &buffer[0], buffer.size(), filename);
		}
		else
		{
			result = luaL_loadfile(L_, filename);
		}

		if (result == 0) result = lua_pcall(L_, 0, 0, -2);

		switch (result) {
		case 0:
			// success
			break;

		case LUA_ERRSYNTAX:
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
		Main().run(strlen(lpstrCmdLine) ? lpstrCmdLine : "pirates/startup.lua");
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
