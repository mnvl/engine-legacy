//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2012 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <boost/test/unit_test.hpp>
#include <util/get_sources_path.h>
#include <util/logger.h>
#include <graphic/material.h>
#include <graphic/scheduler.h>
#include <graphic/d3d9_renderer.h>
#include <graphic/test_fixture.h>
#include "directional_particle_system.h"
#include "cylindrical_particle_system.h"

BOOST_AUTO_TEST_SUITE(particle_system)

BOOST_FIXTURE_TEST_CASE(directional_ps, graphic::TestFixture)
{
	graphic::RendererPtr renderer(new graphic::D3D9Renderer());

	graphic::CameraPtr camera(new graphic::Camera());
	camera->set_orthographic_projection(20.0f, 20.0f * renderer->get_screen_height() / renderer->get_screen_width(), 1.0f, 1000.0f);
	camera->set_eye_lookat_up(math::vec<3>(10, 10, 10), math::vec<3>(0, 0, 0), math::vec<3>(0, 1, 0));

	graphic::SchedulerPtr scheduler(new graphic::Scheduler(renderer));

	renderer->postconstruct();

	boost::shared_ptr<particle_system::DirectionalParticleSystem> ps(
		new particle_system::DirectionalParticleSystem(renderer, scheduler, camera));

	graphic::MaterialPtr material = renderer->create_material();
	material->set_diffuse_texture("test_particle.png");

	ps->get_emitter().size_interval = math::interval<>(2, 2);
	ps->get_emitter().velocity_interval = math::interval<>(10, 20);
	ps->get_emitter().acceleration_interval = math::interval<>(1, 3);
	ps->get_emitter().ttl_interval = math::interval<>(7, 10);
	ps->get_controller().set_emissions_per_sec(math::interval<>(500, 500));
	ps->get_drawer().set_material(material);

	math::scalar total_time = 0;
	double prev_t = SDL_GetTicks();

	for (int i = 0; i < 100; i++)
	{
		graphic::TestFixture::process_events();

		double t = SDL_GetTicks() * 0.001;
		math::scalar dt = math::scalar(t - prev_t);

		total_time += dt;

		ps->move(dt);
		prev_t = t;

		renderer->begin_frame();
		ps->draw();
		scheduler->flush();
		renderer->finish_frame();

		LOG_DEBUG("frame " << i << " (t = " << total_time << ", dt = " << dt <<")");
		LOG_DEBUG("\tnparticles = " << ps->get_keeper().get_particles_count());

		BOOST_ASSERT (ps->get_keeper().get_particles_count() == ps->get_particles_count_limit() ||
			abs(ps->get_keeper().get_particles_count() - 500 * total_time) <= 1);
	}
}

BOOST_FIXTURE_TEST_CASE(cylindrical_ps, graphic::TestFixture)
{
	graphic::RendererPtr renderer(new graphic::D3D9Renderer());

	graphic::CameraPtr camera(new graphic::Camera());
	camera->set_orthographic_projection(20.0f, 20.0f * renderer->get_screen_height() / renderer->get_screen_width(), 1.0f, 1000.0f);
	camera->set_eye_lookat_up(math::vec<3>(10, 10, 10), math::vec<3>(0, 0, 0), math::vec<3>(0, 1, 0));

	graphic::SchedulerPtr scheduler(new graphic::Scheduler(renderer));

	renderer->postconstruct();

	boost::shared_ptr<particle_system::CylindricalParticleSystem> ps(
		new particle_system::CylindricalParticleSystem(renderer, scheduler, camera));

	boost::shared_ptr<graphic::Material> material = renderer->create_material();
	material->set_diffuse_texture("test_particle.png");

	ps->get_emitter().size_interval = math::interval<>(2, 2);
	ps->get_emitter().velocity_interval = math::interval<>(10, 10);
	ps->get_emitter().acceleration_interval = math::interval<>(-10, -10);
	ps->get_emitter().ttl_interval = math::interval<>(1, 1);
	ps->get_emitter().radius_interval = math::interval<>(2, 2);
	ps->get_controller().set_emissions_per_sec(math::interval<>(500, 500));
	ps->get_drawer().set_material(material);

	math::scalar total_time = 0;
	double prev_t = SDL_GetTicks() * 0.001;

	for (int i = 0; i < 100; i++)
	{
		graphic::TestFixture::process_events();

		double t = SDL_GetTicks() * 0.001;
		math::scalar dt = math::scalar(t - prev_t);

		total_time += dt;

		ps->move(dt);
		prev_t = t;

		renderer->begin_frame();
		ps->draw();
		scheduler->flush();
		renderer->finish_frame();

		LOG_DEBUG("frame " << i << " (t = " << total_time << ", dt = " << dt <<")");
		LOG_DEBUG("\tnparticles = " << ps->get_keeper().get_particles_count());
	}
}

BOOST_AUTO_TEST_SUITE_END()
