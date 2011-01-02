//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2011 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/test/unit_test.hpp>
#include <platform/window.h>
#include <platform/timer.h>
#include <util/logger.h>
#include "d3d9_renderer.h"

BOOST_AUTO_TEST_SUITE(renderer)

BOOST_AUTO_TEST_CASE(basic)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test renderer basic"));
	graphic::D3D9RendererPtr renderer(new graphic::D3D9Renderer(window));

	renderer->postconstruct();
	renderer->reset_state();

	double t = platform::Timer::get_current_time();

	while (platform::Timer::get_current_time() - t < 1.0)
	{
		double t0 = platform::Timer::get_current_time();
		window->run_message_loop();
		double t1 = platform::Timer::get_current_time();
		renderer->begin_frame();
		double t2 = platform::Timer::get_current_time();
		renderer->finish_frame();
		double t3 = platform::Timer::get_current_time();

		LOG_DEBUG("message_loop = " << t1 - t0 <<", begin_frame = " << t2 - t1 <<", finish_frame = " << t3 - t2);
	}
}

BOOST_AUTO_TEST_SUITE_END()
