//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/test/unit_test.hpp>
#include <platform/timer.h>
#include <platform/window.h>
#include <graphic/renderer.h>
#include "directional_particle_system.h"

BOOST_AUTO_TEST_SUITE (test_particle_systems)

BOOST_AUTO_TEST_CASE (test_directional_ps)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));
	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));

	platform::Timer timer;
	graphic::ps::DirectionalParticleSystem ps;

	ps.get_emitter().set_origin(math::vec<3>(0, 0, 0));
	ps.get_emitter().set_direction(math::vec<3>(0, 1, 0));
	ps.get_emitter().set_velocity_interval(math::interval<>(10, 20));
	ps.get_emitter().set_acceleration_interval(math::interval<>(-3, -1));
	ps.get_emitter().set_ttl_interval(math::interval<>(5, 10));
	ps.get_controller().set_emissions_per_sec(math::interval<>(100, 100));

	double prev_t = timer.get_current_time();

	for (int i = 0; i < 100 && window->run_message_loop(); i++)
	{
		double t = timer.get_current_time();
		ps.move(math::scalar(t - prev_t));
		prev_t = t;

		renderer->begin_frame();
		ps.draw();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
