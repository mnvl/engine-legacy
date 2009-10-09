
#include <graphic/camera.h>

namespace graphic {

Camera::Camera() {
	set_perspective_projection(math::PI/4, 1.0f, 1.0f, 10000.0f);
	set_eye_lookat_up(math::vec<3>(0,0,0),math::vec<3>(0,0,1),math::vec<3>(0,1,0));
}

void Camera::set_perspective_projection(math::scalar fov,math::scalar aspect,
	math::scalar z_near,math::scalar z_far)
{
	field_of_view	= fov;
	aspect_ratio	= aspect;
	near_plane	= z_near;
	far_plane	= z_far;

	projection_matrix.perspective(fov, aspect, z_near, z_far);
}

void Camera::set_orthographic_projection(math::scalar width,math::scalar height,
	math::scalar z_near,math::scalar z_far)
{
	field_of_view = math::PI/4;
	aspect_ratio = width / height;
	near_plane = z_near;
	far_plane = z_far;

	view_matrix.translation(-width/2,-height/2,0);
	projection_matrix.ortho(width, height, z_near, z_far);
}

void Camera::set_eye_lookat_up(math::vec<3> const &eye_pt, math::vec<3> const &lookat_pt, math::vec<3> const &up_dir)
{
    eye_point = eye_pt;
    lookat_point = lookat_pt;
    up_direction = up_dir;

    view_dir = lookat_point - eye_point;
	view_dir.normalize();

	cross_dir = view_dir ^ up_direction;

	view_matrix.lookat(eye_point,lookat_point,up_direction);

	view_matrix.inverse(billboard_matrix);
    billboard_matrix.ij[3][0] = 0.0f;
    billboard_matrix.ij[3][1] = 0.0f;
    billboard_matrix.ij[3][2] = 0.0f;
}

void Camera::set_position_rotation(math::vec<3> const &pos, math::vec<3> const &rot)
{
	math::matrix<3,3> tf;
	tf.rotation(rot);

	math::vec<3> dir = math::vec<3>(1, 0, 0) * tf;
	math::vec<3> up = math::vec<3>(0, 1, 0) * tf;

	set_eye_lookat_up(pos, pos + dir, up);
}

void Camera::set_position_quaternion(math::vec<3> const &pos, const math::quaternion<> &rot)
{
	math::matrix<3,3> tf;
	tf.rotation(rot);

	math::vec<3> dir = math::vec<3>(1, 0, 0) * tf;
	math::vec<3> up = math::vec<3>(0, 1, 0) * tf;

	set_eye_lookat_up(pos, pos + dir, up);
}

}
