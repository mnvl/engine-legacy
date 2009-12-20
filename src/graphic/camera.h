#pragma once

#include <lua.h>
#include <math/vec.h>
#include <math/matrix.h>

namespace graphic {

class Camera {
public:
	Camera();

    math::matrix<4,4> const &get_view_matrix() const {
		return view_matrix_;
	}

	// TODO: update eye_point_, up_dir_ etc.
	void set_view_matrix(math::matrix<4,4> const &tf) {
		view_matrix_ = tf;
	}

    math::matrix<4,4> const &get_projection_matrix() const {
		return projection_matrix_;
	}

	math::vec<3> const &get_eye_point() const { return eye_point_; }
	math::vec<3> const &get_up_dir() const { return up_dir_; }
	math::vec<3> const &get_view_dir() const { return view_dir_; }
	math::vec<3> const &get_cross_dir() const { return cross_dir_; }

	void set_perspective_projection(math::scalar fFOV,math::scalar fAspect,math::scalar fNearPlane,
		math::scalar fFarPlane);
	void set_orthographic_projection(math::scalar width,math::scalar height,math::scalar fNearPlane,
		math::scalar fFarPlane);

   	void set_eye_lookat_up(math::vec<3> const &eye_pt, math::vec<3> const &lookat_pt, math::vec<3> const &up_dir);
	void set_translation_rotation(math::vec<3> const &tl, math::vec<3> const &rot);

	static void bind(lua_State *L);

private:
	math::vec<3> eye_point_;
	math::vec<3> up_dir_;
	math::vec<3> view_dir_;
	math::vec<3> cross_dir_;

	math::matrix<4,4> view_matrix_;
	math::matrix<4,4> billboard_matrix_;

	math::scalar field_of_view_;
	math::scalar aspect_ratio_;
	math::scalar near_plane_;
	math::scalar far_plane_;

	math::matrix<4,4> projection_matrix_;
};

}
