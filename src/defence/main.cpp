
#include <xercesc/util/PlatformUtils.hpp>
#include <windows.h>
#include <util/logger.h>
#include "main.h"

Main::Main()
{
    xercesc::XMLPlatformUtils::Initialize();

    window_.reset(new platform::Window("defence"));
    renderer_.reset(new graphic::Renderer(window_.get()));
    lua_state_.reset(new LuaState());
}

void Main::run()
{
    lua_state_->dofile("startup.lua");

    while (window_->run_message_loop())
    {
        renderer_->begin_frame();

        renderer_->finish_frame();
    }
}

Main::~Main()
{
    xercesc::XMLPlatformUtils::Terminate();
}

Main &Main::instance()
{
    static Main main;
    return main;
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpstrCmdLine, INT nShowCmd)
{
    try
    {
        Main::instance().run();
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
