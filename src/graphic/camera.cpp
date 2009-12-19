
#include <d3dx9.h>
#include <graphic/camera.h>

namespace graphic
{

Camera::Camera() {
	set_perspective_projection(math::PI/4, 1.0f, 1.0f, 10000.0f);
	set_eye_lookat_up(math::vec<3>(0, 0, 0), math::vec<3>(0, 0, 1), math::vec<3>(0, 1, 0));
}

void Camera::set_perspective_projection(math::scalar fov, math::scalar aspect, math::scalar z_near, math::scalar z_far)
{
	field_of_view_ = fov;
	aspect_ratio_ = aspect;
	near_plane_ = z_near;
	far_plane_ = z_far;

	projection_matrix_.perspective(fov, aspect, z_near, z_far);
}

void Camera::set_orthographic_projection(math::scalar width, math::scalar height, math::scalar z_near, math::scalar z_far)
{
	field_of_view_ = math::PI/4;
	aspect_ratio_ = width / height;
	near_plane_ = z_near;
	far_plane_ = z_far;

	projection_matrix_.ortho(width, height, z_near, z_far);
	view_matrix_.identity();
}

void Camera::set_eye_lookat_up(math::vec<3> const &eye_pt, math::vec<3> const &lookat_pt, math::vec<3> const &up_dir)
{
    eye_point_ = eye_pt;
    lookat_point_ = lookat_pt;
    up_direction_ = up_dir;

    view_dir_ = lookat_point_ - eye_point_;
	view_dir_.normalize();

	cross_dir_ = view_dir_ ^ up_direction_;

	view_matrix_.lookat(eye_point_, lookat_point_, up_direction_);

	view_matrix_.inverse(billboard_matrix_);
    billboard_matrix_.ij[3][0] = 0.0f;
    billboard_matrix_.ij[3][1] = 0.0f;
    billboard_matrix_.ij[3][2] = 0.0f;
}

void Camera::set_position_rotation(math::vec<3> const &pos, math::vec<3> const &rot)
{
	math::matrix<3, 3> tf;
	tf.rotation(rot);

	math::vec<3> dir = math::vec<3>(1, 0, 0) * tf;
	math::vec<3> up = math::vec<3>(0, 1, 0) * tf;

	set_eye_lookat_up(pos, pos + dir, up);
}

void Camera::set_position_quaternion(math::vec<3> const &pos, const math::quaternion<> &rot)
{
	math::matrix<3, 3> tf;
	tf.rotation(rot);

	math::vec<3> dir = math::vec<3>(1, 0, 0) * tf;
	math::vec<3> up = math::vec<3>(0, 1, 0) * tf;

	set_eye_lookat_up(pos, pos + dir, up);
}

}
