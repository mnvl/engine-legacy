
#include <boost/test/unit_test.hpp>
#include <platform/window.h>
#include "renderer.h"

BOOST_AUTO_TEST_SUITE (test_window_resize)

BOOST_AUTO_TEST_CASE (test_basic)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));
	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));

	for (int i = 0; i < 10 && window->run_message_loop(); i++)
	{
		if (i == 5) MoveWindow(window->get_handle(), 100, 100, 900, 700, TRUE);

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_CASE (test_with_postconstruct)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));

	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));
	renderer->postconstruct();

	for (int i = 0; i < 10 && window->run_message_loop(); i++)
	{
		if (i == 5) MoveWindow(window->get_handle(), 100, 100, 900, 700, TRUE);

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_CASE (test_with_postconstruct_and_vertex_array)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));

	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));
	renderer->postconstruct();

	boost::shared_ptr<graphic::VertexArrayPT> vertex_array1;
	boost::shared_ptr<graphic::VertexArrayPT> vertex_array2;
	boost::shared_ptr<graphic::VertexArrayPT> vertex_array3;
	boost::shared_ptr<graphic::VertexArrayPT> vertex_array4;


	for (int i = 0; i < 20 && window->run_message_loop(); i++)
	{
		if (i == 5)
		{
			vertex_array1 = renderer->create_vertex_array<graphic::vertex_pt>(100, false, false);
			vertex_array1 = renderer->create_vertex_array<graphic::vertex_pt>(100, false, true);
			vertex_array1 = renderer->create_vertex_array<graphic::vertex_pt>(100, true, false);
			vertex_array1 = renderer->create_vertex_array<graphic::vertex_pt>(100, true, true);
		}

		if (i == 15)
		{
			MoveWindow(window->get_handle(), 100, 100, 900, 700, TRUE);
		}

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_CASE (test_with_postconstruct_and_texture)
{
	boost::shared_ptr<platform::Window> window(new platform::Window("test_window_resize"));

	boost::shared_ptr<graphic::Renderer> renderer(new graphic::Renderer(window));
	renderer->postconstruct();

	boost::shared_ptr<graphic::Texture> texture;

	for (int i = 0; i < 20 && window->run_message_loop(); i++)
	{
		if (i == 5)
		{
			texture = renderer->create_texture("test_white.tga");
		}

		if (i == 15)
		{
			MoveWindow(window->get_handle(), 100, 100, 900, 700, TRUE);
		}

		renderer->begin_frame();
		renderer->finish_frame();
	}
}

BOOST_AUTO_TEST_SUITE_END()
