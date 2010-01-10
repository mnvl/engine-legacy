#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

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

	virtual void handle_lost_device() = 0;
	virtual void handle_reset_device() = 0;

	static void bind(lua_State *L);
};

typedef boost::shared_ptr<Resource> ResourcePtr;
typedef boost::weak_ptr<Resource> ResourceWeakPtr;

class ResourcesDirectory
{
public:
	ResourcesDirectory();

	void add_resource(ResourcePtr const &resource);
	void handle_lost_device();
	void handle_reset_device();
	void cleanup();

private:
	typedef std::deque<ResourceWeakPtr> storage_t;
	storage_t storage_;

	static size_t const RESOURCES_ADDED_AFTER_LAST_CLEANUP_THRESHOLD = 4096;
	size_t resources_added_after_last_cleanup_;
};

}
