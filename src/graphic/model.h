#pragma once

#include <boost/ptr_container/ptr_vector.hpp>
#include <graphic/joint.h>
#include <graphic/animation.h>
#include <graphic/mesh.h>

namespace graphic {

class Model {
public:
	typedef boost::ptr_vector<Joint> joints_type;
	typedef boost::ptr_vector<Animation> animations_type;
	typedef boost::ptr_vector<Mesh> meshes_type;
	typedef boost::ptr_vector<Material> materials_type;

	joints_type joints;
	meshes_type meshes;
	animations_type animations;
	materials_type materials;

	Model(Renderer *renderer_ptr = 0);
	virtual ~Model();

	Renderer *get_renderer() const
	{
		return renderer_ptr_;
	}

	Joint *get_joint_by_id(std::string const &name);
	Joint *get_joint_by_name(std::string const &name);
	Mesh *get_mesh_by_id(std::string const &id);
	Mesh *get_mesh_by_name(std::string const &name);
	Material *get_material_by_name(std::string const &name);

	virtual Joint *create_joint();
	virtual Mesh *create_mesh() = 0;

    void load_from_collada(std::string const &filename);
    void load_from_raw_i16_heightfield(std::string const &filename, math::matrix<4,4> const &tf,
		size_t nrows, size_t ncols, math::vec<3> const &scale, std::string const &texture_name);
    void load_from_raw_u16_heightfield(std::string const &filename, math::matrix<4,4> const &tf,
		size_t nrows, size_t ncols, math::vec<3> const &scale, std::string const &texture_name);

    virtual void set_animation_time(math::scalar t);
    virtual void draw();

protected:
	Renderer *renderer_ptr_;
};

}
