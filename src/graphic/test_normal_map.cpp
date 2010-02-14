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
#include <platform/window.h>
#include <util/get_sources_path.h>
#include "material.h"
#include "renderer.h"

BOOST_AUTO_TEST_SUITE(normal_map)

BOOST_AUTO_TEST_CASE(basic)
{
	std::string const &src = util::get_graphic_sources_path();

	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));
	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));

	boost::shared_ptr<graphic::Material> material(new graphic::Material(renderer));
	material->set_diffuse_texture(src + "test_white.png");
	material->set_normal_map_texture(src + "test_particle.png");

	for (int i = 0; i < 1000 && window->run_message_loop(); i++)
	{
		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
