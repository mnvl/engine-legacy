#pragma once

#include <boost/scoped_ptr.hpp>
#include <platform/window.h>
#include <graphic/renderer.h>

namespace bootstrap
{

class global {
public:
    global();
    ~global();

    void init();
    void run();

    static global &instance();

private:
    boost::scoped_ptr<platform::window> window_;
    boost::scoped_ptr<graphic::renderer> renderer_;
};

}
