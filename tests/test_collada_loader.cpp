
#include <engine/graphic/collada_loader.h>
#include <cppunit/extensions/HelperMacros.h>

class test_collada_loader: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(test_collada_loader);
	CPPUNIT_TEST(test_plane);
	CPPUNIT_TEST(test_joints);
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

	void test_joints()
	{
		engine::graphic::collada_loader cl("test_collada_loader_joints.xml");
		cl.load();

		engine::graphic::memory_model const *model_ptr = &cl.get_model();
		CPPUNIT_ASSERT (model_ptr->meshes.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints.size() == 2);
		CPPUNIT_ASSERT (model_ptr->animations.size() == 0);

		engine::graphic::memory_mesh const *mesh_ptr = &model_ptr->meshes.front();

		std::vector<engine::graphic::memory_mesh::vertex> vertices;
		mesh_ptr->transform_vertices(vertices);

		CPPUNIT_ASSERT(vertices.size() == 4);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_collada_loader); 