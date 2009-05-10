#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include <math/matrix.h>

namespace scene {

class node {
public:
	node();
	~node();

	void add_child(std::auto_ptr<node> &child_ptr);

private:
	typedef boost::ptr_vector<node> childs_type;

	node *parent_ptr_;
	childs_type childs_;
};

}

