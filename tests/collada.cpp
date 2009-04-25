
#include <engine/graphic/collada_loader.h>
#include <cppunit/extensions/HelperMacros.h>

class test_collada_loader: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(test_collada_loader);
	CPPUNIT_TEST(test_plane_1);
	CPPUNIT_TEST_SUITE_END();

	void test_plane_1()
	{
		engine::graphic::collada_loader cl("collada_plane1.xml");
		cl.load();

		engine::graphic::memory_model const *model_ptr = &cl.get_model();
		CPPUNIT_ASSERT (model_ptr->meshes.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints.size() == 0);
		CPPUNIT_ASSERT (model_ptr->animations.size() == 0);

		{
			math::matrix<4,4> tf;
			tf.translation(math::vec<3>(1.0f, -0.5f, 3.0f));
			CPPUNIT_ASSERT(model_ptr->transform == tf);
		}

		engine::graphic::memory_mesh const *mesh_ptr = &model_ptr->meshes.front();
		CPPUNIT_ASSERT (mesh_ptr->vertices.size() == 6);
		CPPUNIT_ASSERT (mesh_ptr->indices.size() == 6);


	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_collada_loader); 