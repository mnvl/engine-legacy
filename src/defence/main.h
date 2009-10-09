#pragma once

#include <boost/scoped_ptr.hpp>
#include <luabind/lua_include.hpp>
#include <platform/window.h>
#include <graphic/renderer.h>
#include "lua_state.h"

class Main
{
public:
    Main();
    ~Main();

    void run();

    static Main &instance();
    static platform::Window *get_window() { return instance().window_.get(); }
    static graphic::Renderer *get_renderer() { return instance().renderer_.get(); }

    static void bind(lua_State *L);

private:
    boost::scoped_ptr<platform::Window> window_;
    boost::scoped_ptr<graphic::Renderer> renderer_;
    boost::scoped_ptr<LuaState> lua_state_;
};
