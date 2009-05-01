
#include <graphic/camera.h>

namespace engine {
namespace graphic {

camera::camera() {
   	// Set attributes for the view matrix
	set_position(math::vec<3>(0,0,0),math::vec<3>(0,0,1),math::vec<3>(0,1,0));

   	// Set attributes for the projection matrix
	set_perspective_projection(math::PI/4, 1.0f, 1.0f, 10000.0f);
}

void camera::set_position(math::vec<3> const &eye_pt,math::vec<3> const &lookat_pt,
	math::vec<3> const &up_dir)
{
	// Set attributes for the view matrix
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

void camera::set_position(math::vec<3> const &pos,math::vec<3> const &rot) {
	math::matrix<3,3> tf;
	tf.rotation(rot);

	math::vec<3> dir = math::vec<3>(1, 0, 0) * tf;
	math::vec<3> up = math::vec<3>(0, 1, 0) * tf;
	
	set_position(pos, pos + dir, up);
}

void camera::set_position(math::vec<3> const &pos,const math::quaternion<> &rot) {
	math::matrix<3,3> tf;
	tf.rotation(rot);

	math::vec<3> dir = math::vec<3>(1, 0, 0) * tf;
	math::vec<3> up = math::vec<3>(0, 1, 0) * tf;
	
	set_position(pos, pos + dir, up);
}

void camera::set_perspective_projection(math::scalar fov,math::scalar aspect,
	math::scalar z_near,math::scalar z_far)
{
	field_of_view	= fov;
	aspect_ratio	= aspect;
	near_plane	= z_near;
	far_plane	= z_far;

	projection_matrix.perspective(fov, aspect, z_near, z_far);
}

void camera::set_orthographic_projection(math::scalar width,math::scalar height,
	math::scalar z_near,math::scalar z_far)
{
	field_of_view = math::PI/4;
	aspect_ratio = width / height;
	near_plane = z_near;
	far_plane = z_far;

	view_matrix.translation(-width/2,-height/2,0);
	projection_matrix.ortho(width, height, z_near, z_far);
}

}
}