
#include <crtdbg.h>
#include <windows.h>
#include <lualib.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <luabind/luabind.hpp>
#include <luabind/detail/pcall.hpp>
#include <graphic/bind.h>
#include <platform/timer.h>
#include <platform/window.h>
#include <platform/input.h>
#include <platform/keyboard.h>
#include <platform/mouse.h>
#include <math/bind.h>
#include <util/logger.h>
#include <util/lua_backtrace.h>

LUALIB_API int luaopen_bit(lua_State *L);

class Main
{
public:
    Main():
		L_(lua_open())
	{
		xercesc::XMLPlatformUtils::Initialize();

		luaL_openlibs(L_);
		luaopen_bit(L_);
		luabind::open(L_);

		graphic::bind(L_);

		platform::Timer::bind(L_);
		platform::Window::bind(L_);

		math::bind(L_);

		util::Logger::bind(L_);

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
		int result = luaL_loadfile(L_, filename);
		if (result == 0) result = luabind::detail::pcall(L_, 0, 0);

		switch (result)
		{
		case 0:
			// success
			break;

		case LUA_ERRSYNTAX:
			throw std::runtime_error("syntax eror");

		case LUA_ERRRUN:
			throw std::runtime_error(lua_tostring(L_, -1));

		default:
			throw std::runtime_error("unknown error");
		}
	}

private:
    lua_State *L_;
};


INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
    try
    {
		luabind::set_pcall_callback(util::make_lua_backtrace);

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
