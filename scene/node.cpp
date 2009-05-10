
#include <scene/node.h>

namespace scene {

node::node():
parent_ptr_(0)
{
}

node::~node()
{
}

void node::add_child(std::auto_ptr<node> &child_ptr)
{
	child_ptr->parent_ptr_ = this;
	childs_.push_back(child_ptr);
}

}


