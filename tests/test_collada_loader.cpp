
#include <engine/graphic/collada_loader.h>
#include <cppunit/extensions/HelperMacros.h>

class test_collada_loader: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(test_collada_loader);
	CPPUNIT_TEST(test_plane);
	CPPUNIT_TEST(test_joint);
	CPPUNIT_TEST(test_animation);
	CPPUNIT_TEST_SUITE_END();

	void test_plane()
	{
		engine::graphic::collada_loader cl("test_collada_loader_plane.xml");
		cl.load();

		engine::graphic::memory_model const *model_ptr = &cl.get_model();
		CPPUNIT_ASSERT (model_ptr->meshes.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints.size() == 0);
		CPPUNIT_ASSERT (model_ptr->animations.size() == 0);

		{
			math::matrix<4,4> tf;
			tf.identity();
			CPPUNIT_ASSERT(model_ptr->transform == tf);
		}

		engine::graphic::memory_mesh const *mesh_ptr = &model_ptr->meshes.front();
		CPPUNIT_ASSERT (mesh_ptr->vertices.size() == 4);
		CPPUNIT_ASSERT (mesh_ptr->indices.size() == 6);

		{
			math::matrix<4,4> tf;
			tf.translation(math::vec<3>(1.0f, -0.5f, 3.0f));
			CPPUNIT_ASSERT (mesh_ptr->transform == tf);
		}

		CPPUNIT_ASSERT ((mesh_ptr->vertices[0].position - math::vec<3>(-0.5f, 0.0f,  0.5f)).length_sq() < math::EPSILON);
		CPPUNIT_ASSERT ((mesh_ptr->vertices[1].position - math::vec<3>( 0.5f, 0.0f,  0.5f)).length_sq() < math::EPSILON);
		CPPUNIT_ASSERT ((mesh_ptr->vertices[2].position - math::vec<3>(-0.5f, 0.0f, -0.5f)).length_sq() < math::EPSILON);
		CPPUNIT_ASSERT ((mesh_ptr->vertices[3].position - math::vec<3>( 0.5f, 0.0f, -0.5f)).length_sq() < math::EPSILON);

		CPPUNIT_ASSERT (mesh_ptr->indices[0] == 0);
		CPPUNIT_ASSERT (mesh_ptr->indices[1] == 1);
		CPPUNIT_ASSERT (mesh_ptr->indices[2] == 2);
		CPPUNIT_ASSERT (mesh_ptr->indices[3] == 2);
		CPPUNIT_ASSERT (mesh_ptr->indices[4] == 1);
		CPPUNIT_ASSERT (mesh_ptr->indices[5] == 3);
	}

	void test_joint()
	{
		engine::graphic::collada_loader cl("test_collada_loader_joint.xml");
		cl.load();

		engine::graphic::memory_model *model_ptr = const_cast<engine::graphic::memory_model *>(&cl.get_model());
		CPPUNIT_ASSERT (model_ptr->meshes.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints.size() == 1);
		CPPUNIT_ASSERT (model_ptr->animations.size() == 1);

		CPPUNIT_ASSERT (model_ptr->joints[0].parent == 0);

		engine::graphic::memory_mesh *mesh_ptr = &model_ptr->meshes.front();

		model_ptr->set_animation_time(0);
		std::vector<engine::graphic::memory_mesh::vertex> vertices1;
		mesh_ptr->transform_vertices(vertices1);

		CPPUNIT_ASSERT (vertices1.size() == 3);
		CPPUNIT_ASSERT (abs(vertices1[0].position.y) < 0.1f);

		model_ptr->set_animation_time(1);
		std::vector<engine::graphic::memory_mesh::vertex> vertices2;
		mesh_ptr->transform_vertices(vertices2);

		CPPUNIT_ASSERT (vertices2.size() == 3);
		CPPUNIT_ASSERT (abs(vertices2[0].position.y - 1.0f) < 0.1f);
	}

	void test_animation()
	{
		engine::graphic::collada_loader cl("test_collada_loader_animation.xml");
		cl.load();

		engine::graphic::memory_model *model_ptr = const_cast<engine::graphic::memory_model *>(&cl.get_model());
		CPPUNIT_ASSERT (model_ptr->meshes.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints.size() == 3);
		CPPUNIT_ASSERT (model_ptr->animations.size() == 1);

		CPPUNIT_ASSERT (model_ptr->joints[0].parent == 0);
		CPPUNIT_ASSERT (model_ptr->joints[1].parent == &model_ptr->joints[0]);
		CPPUNIT_ASSERT (model_ptr->joints[2].parent == &model_ptr->joints[1]);

		CPPUNIT_ASSERT (model_ptr->joints[0].childs.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints[0].childs[0] == &model_ptr->joints[1]);

		CPPUNIT_ASSERT (model_ptr->joints[1].childs.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints[1].childs[0] == &model_ptr->joints[2]);

		CPPUNIT_ASSERT (model_ptr->joints[2].childs.size() == 0);

		engine::graphic::memory_mesh *mesh_ptr = &model_ptr->meshes.front();

		model_ptr->set_animation_time(0);
		std::vector<engine::graphic::memory_mesh::vertex> vertices1;
		mesh_ptr->transform_vertices(vertices1);

		model_ptr->set_animation_time(1);
		std::vector<engine::graphic::memory_mesh::vertex> vertices2;
		mesh_ptr->transform_vertices(vertices2);

		CPPUNIT_ASSERT((vertices1[0].position - math::vec<3>(-1.0f, 0.85f, 1.0f)).length_sq() < 0.1f);
		CPPUNIT_ASSERT((vertices2[0].position - math::vec<3>(-1.0f, 0.85f, 1.0f)).length_sq() < 0.1f);

		CPPUNIT_ASSERT((vertices1[3].position - math::vec<3>(1.0f, 1.6f, -1.0f)).length_sq() < 0.1f);
		CPPUNIT_ASSERT((vertices2[3].position - math::vec<3>(2.0f, 0.0f, -0.3f)).length_sq() < 0.1f);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_collada_loader); 