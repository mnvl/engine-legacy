#pragma once

#include <math/vec.h>
#include <math/matrix.h>

namespace graphic {

class Camera {
private:
	math::vec<3> eye_point;
	math::vec<3> lookat_point;
	math::vec<3> up_direction;

	math::vec<3> view_dir;
	math::vec<3> cross_dir;

	math::matrix<4,4> view_matrix;
	math::matrix<4,4> billboard_matrix;

	math::scalar field_of_view;
	math::scalar aspect_ratio;
	math::scalar near_plane;
	math::scalar far_plane;

	math::matrix<4,4>  projection_matrix;

public:
	Camera();

    math::matrix<4,4> const &get_view_matrix() const {
		return view_matrix;
	}

    math::matrix<4,4> const &get_projection_matrix() const {
		return projection_matrix;
	}

	void set_perspective_projection(math::scalar fFOV,math::scalar fAspect,math::scalar fNearPlane,
		math::scalar fFarPlane);
	void set_orthographic_projection(math::scalar width,math::scalar height,math::scalar fNearPlane,
		math::scalar fFarPlane);

   	void set_eye_lookat_up(math::vec<3> const &eye_pt, math::vec<3> const &lookat_pt, math::vec<3> const &up_dir);
	void set_position_rotation(math::vec<3> const &pos, math::vec<3> const &rot);
	void set_position_quaternion(math::vec<3> const &pos, const math::quaternion<> &rot);
};

}
