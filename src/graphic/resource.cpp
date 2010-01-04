
#include <luabind/luabind.hpp>
#include "resource.h"

namespace graphic
{

Resource::~Resource()
{
}

void Resource::bind(lua_State *L)
{
	using namespace luabind;

	module(L, "graphic")
	[
		class_<Resource, boost::shared_ptr<Resource> >("Resource")
	];
}

ResourcesDirectory::ResourcesDirectory():
	resources_added_after_last_cleanup_(0)
{
}

void ResourcesDirectory::add_resource(ResourcePtr const &resource)
{
	storage_.push_back(resource);

	if (++resources_added_after_last_cleanup_ > RESOURCES_ADDED_AFTER_LAST_CLEANUP_THRESHOLD)
	{
		cleanup();
	}
}

void ResourcesDirectory::handle_lost_device()
{
	storage_t::iterator it = storage_.begin();

	while (it != storage_.end())
	{
		if (ResourcePtr const &resource = it->lock())
		{
			resource->handle_lost_device();
			++it;
		}
		else
		{
			it = storage_.erase(it);
		}
	}

	resources_added_after_last_cleanup_ = 0;
}

void ResourcesDirectory::handle_reset_device()
{
	storage_t::iterator it = storage_.begin();

	while (it != storage_.end())
	{
		if (ResourcePtr const &resource = it->lock())
		{
			resource->handle_reset_device();
			++it;
		}
		else
		{
			it = storage_.erase(it);
		}
	}

	resources_added_after_last_cleanup_ = 0;
}

void ResourcesDirectory::cleanup()
{
	storage_t::iterator it = storage_.begin();

	while (it != storage_.end())
	{
		if (it->lock())
		{
			++it;
		}
		else
		{
			it = storage_.erase(it);
		}
	}

	resources_added_after_last_cleanup_ = 0;
}

}
