#pragma once

#include <deque>
#include <boost/noncopyable.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <lua.h>

namespace graphic
{

class Resource: public boost::noncopyable
{
public:
	virtual ~Resource();

	virtual void handle_device_lost() = 0;
	virtual void restore_after_device_lost() = 0;

	static void bind(lua_State *L);
};

typedef boost::shared_ptr<Resource> ResourcePtr;
typedef boost::weak_ptr<Resource> ResourceWeakPtr;

class ResourcesDirectory
{
public:
	ResourcesDirectory();

	void add_resource(ResourcePtr const &resource);
	void handle_device_lost();
	void restore_after_device_lost();
	void cleanup();

private:
	typedef std::deque<ResourceWeakPtr> storage_t;
	storage_t storage_;

	static size_t const RESOURCES_ADDED_AFTER_LAST_CLEANUP_THRESHOLD = 4096;
	size_t resources_added_after_last_cleanup_;
};

}
