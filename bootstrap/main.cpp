
#include <windows.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <luabind/luabind.hpp>
#include <platform/window.h>
#include <graphic/renderer.h>
#include <util/logger.h>

namespace bootstrap
{
void bind_math(lua_State *L);
void bind_platform(lua_State *L);
void bind_graphic(lua_State *L);
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
	try
	{
		util::logger::instance().set_stderr();

		xercesc::XMLPlatformUtils::Initialize();

        lua_State *L = luaL_newstate();
        luaL_openlibs(L);
        luabind::open(L);
        bootstrap::bind_math(L);
        bootstrap::bind_platform(L);
        bootstrap::bind_graphic(L);

        if (luaL_dofile(L, lpstrCmdLine) != 0)
        {
            throw std::runtime_error(lua_tostring(L, -1));
        }

        lua_close(L);

		xercesc::XMLPlatformUtils::Terminate();
	}
	catch(std::exception const &error)
	{
        LOG_CRITICAL("exception caught: " << error.what());
		MessageBox(0, error.what(), 0, MB_OK);
		return 1;
	}
	catch (xercesc::XMLException const &error)
	{
		char* message = xercesc::XMLString::transcode(error.getMessage());
        LOG_CRITICAL("exception caught: " << message);
		MessageBox(0, message, 0, MB_OK);
        xercesc::XMLString::release(&message);
		return 1;
    }
	catch (xercesc::DOMException const &error)
	{
		char* message = xercesc::XMLString::transcode(error.msg);
        LOG_CRITICAL("exception caught: " << message);
		MessageBox(0, message, 0, MB_OK);
        xercesc::XMLString::release(&message);
		return 1;
    }

    LOG_INFO("terminated with success");

	return 0;
}
