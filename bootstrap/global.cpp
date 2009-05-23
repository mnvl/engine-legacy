
#include <bootstrap/global.h>

namespace bootstrap
{

global::global()
{
}

global::~global()
{
}

void global::init()
{
    window_.reset(new platform::window("bootstrap"));
    renderer_.reset(new graphic::renderer(window_.get()));
}

void global::run()
{
    while (window_->run_message_loop())
    {
        renderer_->begin_frame();
        renderer_->finish_frame();
    }
}

global &global::instance()
{
    static global singleton;
    return singleton;
}

}
