
#include <graphic/collada_loader.h>
#include <cppunit/extensions/HelperMacros.h>

class test_collada_loader: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(test_collada_loader);
	CPPUNIT_TEST(test_plane);
	CPPUNIT_TEST(test_joint);
	CPPUNIT_TEST(test_animation);
	CPPUNIT_TEST(test_materials);
	CPPUNIT_TEST_SUITE_END();

	void test_plane()
	{
		graphic::collada_loader cl("test_collada_loader_plane.xml");
		cl.load();

		graphic::memory_model const *model_ptr = &cl.get_model();
		CPPUNIT_ASSERT (model_ptr->meshes.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints.size() == 1);
		CPPUNIT_ASSERT (model_ptr->animations.size() == 0);

		CPPUNIT_ASSERT (model_ptr->joints[0].mesh_ptr == &model_ptr->meshes[0]);

		graphic::memory_mesh const *mesh_ptr = (graphic::memory_mesh const *) &model_ptr->meshes.front();
		CPPUNIT_ASSERT (mesh_ptr->vertices.size() == 4);
		CPPUNIT_ASSERT (mesh_ptr->indices.size() == 6);

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
		graphic::collada_loader cl("test_collada_loader_joint.xml");
		cl.load();

		graphic::memory_model *model_ptr = const_cast<graphic::memory_model *>(&cl.get_model());
		CPPUNIT_ASSERT (model_ptr->meshes.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints.size() == 2);
		CPPUNIT_ASSERT (model_ptr->animations.size() == 1);

		CPPUNIT_ASSERT (model_ptr->joints[0].parent == 0);
		CPPUNIT_ASSERT (model_ptr->joints[1].parent == 0);

		CPPUNIT_ASSERT (model_ptr->joints[0].mesh_ptr == (graphic::mesh *) &model_ptr->meshes[0]);

		graphic::memory_mesh *mesh_ptr = (graphic::memory_mesh *) &model_ptr->meshes.front();

		model_ptr->set_animation_time(0);
		std::vector<graphic::memory_mesh::vertex_t> vertices1;
		mesh_ptr->transform_vertices(&model_ptr->joints[0], vertices1);

		CPPUNIT_ASSERT (vertices1.size() == 3);
		CPPUNIT_ASSERT (abs(vertices1[0].position.y) < 0.1f);

		model_ptr->set_animation_time(1);
		std::vector<graphic::memory_mesh::vertex_t> vertices2;
		mesh_ptr->transform_vertices(&model_ptr->joints[0], vertices2);

		CPPUNIT_ASSERT (vertices2.size() == 3);
		CPPUNIT_ASSERT (abs(vertices2[0].position.y - 1.0f) < 0.1f);
	}

	void test_animation()
	{
		graphic::collada_loader cl("test_collada_loader_animation.xml");
		cl.load();

		graphic::memory_model *model_ptr = const_cast<graphic::memory_model *>(&cl.get_model());
		CPPUNIT_ASSERT (model_ptr->meshes.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints.size() == 4);
		CPPUNIT_ASSERT (model_ptr->animations.size() == 1);

		CPPUNIT_ASSERT (model_ptr->joints[0].parent == 0);
		CPPUNIT_ASSERT (model_ptr->joints[1].parent == 0);
		CPPUNIT_ASSERT (model_ptr->joints[2].parent == &model_ptr->joints[1]);
		CPPUNIT_ASSERT (model_ptr->joints[3].parent == &model_ptr->joints[2]);

		CPPUNIT_ASSERT (model_ptr->joints[0].childs.size() == 0);

		CPPUNIT_ASSERT (model_ptr->joints[1].childs.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints[1].childs[0] == &model_ptr->joints[2]);

		CPPUNIT_ASSERT (model_ptr->joints[2].childs.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints[2].childs[0] == &model_ptr->joints[3]);

		CPPUNIT_ASSERT (model_ptr->joints[3].childs.size() == 0);

		graphic::memory_mesh *mesh_ptr = (graphic::memory_mesh *) &model_ptr->meshes.front();
		CPPUNIT_ASSERT (model_ptr->joints[0].mesh_ptr == mesh_ptr);

		model_ptr->set_animation_time(0);
		std::vector<graphic::memory_mesh::vertex_t> vertices1;
		mesh_ptr->transform_vertices(&model_ptr->joints[0], vertices1);

		model_ptr->set_animation_time(1);
		std::vector<graphic::memory_mesh::vertex_t> vertices2;
		mesh_ptr->transform_vertices(&model_ptr->joints[0], vertices2);

		CPPUNIT_ASSERT((vertices1[0].position - math::vec<3>(-1.0f, 0.85f, 1.0f)).length_sq() < 0.1f);
		CPPUNIT_ASSERT((vertices2[0].position - math::vec<3>(-1.0f, 0.85f, 1.0f)).length_sq() < 0.1f);

		CPPUNIT_ASSERT((vertices1[3].position - math::vec<3>(1.0f, 1.6f, -1.0f)).length_sq() < 0.1f);
		CPPUNIT_ASSERT((vertices2[3].position - math::vec<3>(2.0f, 0.0f, -0.3f)).length_sq() < 0.1f);
	}

	void test_materials()
	{
		graphic::collada_loader cl("test_collada_loader_materials.xml");
		cl.load();

		graphic::memory_model const *model_ptr = &cl.get_model();
		CPPUNIT_ASSERT (model_ptr->meshes.size() == 1);
		CPPUNIT_ASSERT (model_ptr->joints.size() == 1);
		CPPUNIT_ASSERT (model_ptr->animations.size() == 0);

		CPPUNIT_ASSERT (model_ptr->joints[0].mesh_ptr == &model_ptr->meshes[0]);

		graphic::material const &m0 = model_ptr->materials[0];
		CPPUNIT_ASSERT (m0.get_texture_name() == "file2.tga");

		graphic::material const &m1 = model_ptr->materials[1];
		CPPUNIT_ASSERT (m1.get_texture_name() == "file1.tga");

		graphic::material const &m2 = model_ptr->materials[2];
		CPPUNIT_ASSERT (m2.get_texture_name() == "file3.tga");

		graphic::memory_mesh const *mesh_ptr = (graphic::memory_mesh const *) &model_ptr->meshes.front();
		CPPUNIT_ASSERT (mesh_ptr->vertices.size() == 9);
		CPPUNIT_ASSERT (mesh_ptr->indices.size() == 9);

		graphic::mesh::triangles const &t0 = mesh_ptr->triangles_list[0];
		CPPUNIT_ASSERT (t0.material_ptr->get_name() == "lambert1");
		CPPUNIT_ASSERT (t0.triangles_count == 1);
		CPPUNIT_ASSERT (t0.first_index_offset == 0);
		CPPUNIT_ASSERT (t0.minimal_vertex_index == 0);
		CPPUNIT_ASSERT (t0.vertices_count == 3);

		graphic::mesh::triangles const &t1 = mesh_ptr->triangles_list[1];
		CPPUNIT_ASSERT (t1.material_ptr->get_name() == "phong1");
		CPPUNIT_ASSERT (t1.triangles_count == 1);
		CPPUNIT_ASSERT (t1.first_index_offset == 3);
		CPPUNIT_ASSERT (t1.minimal_vertex_index == 3);
		CPPUNIT_ASSERT (t1.vertices_count == 3);

		graphic::mesh::triangles const &t2 = mesh_ptr->triangles_list[2];
		CPPUNIT_ASSERT (t2.material_ptr->get_name() == "blinn1");
		CPPUNIT_ASSERT (t2.triangles_count == 1);
		CPPUNIT_ASSERT (t2.first_index_offset == 6);
		CPPUNIT_ASSERT (t2.minimal_vertex_index == 6);
		CPPUNIT_ASSERT (t2.vertices_count == 3);
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(test_collada_loader); 