//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include "draw_call.h"
#include "renderer.h"
#include "scheduler.h"

namespace graphic
{

Scheduler::Scheduler(boost::shared_ptr<Renderer> const &renderer):
	renderer_(renderer)
{
	draw_calls_.reserve(4096);
}

Scheduler::~Scheduler()
{
}

void Scheduler::add(boost::shared_ptr<DrawCall> const &draw_call)
{
	draw_calls_.push_back(draw_call);
}

void Scheduler::flush()
{
	for(draw_calls_t::const_iterator it = draw_calls_.begin(); it != draw_calls_.end(); ++it)
	{
		(*it)->execute();
	}

	draw_calls_.clear();
}

}
