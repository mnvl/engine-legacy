
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <math/vec.h>

template class math::vec<2>;
template class math::vec<3>;
template class math::vec<4>;

template<class T> void
math::vec<2,T>::to_polar(scalar_type &modulus,scalar_type &angle) const {
	modulus = length();
	angle = atan2(y, x);
}

template<class T> void
math::vec<3,T>::to_axis_angle(vec_type &axis, scalar_type &angle) const {
	const vec_type original(1,0,0);

	axis = original ^ *this;
	axis.normalize();

	vec_type basis_y = axis ^ original;

	vec<2,T> xy;
	xy.x = x / length();
	xy.y = *this & basis_y;

	scalar_type t;
	xy.to_polar(t, angle);
}

void math::bind_vec(lua_State *L)
{
	using namespace luabind;

	module(L, "math")
	[
		class_<vec<2> >("vec2")
		.def(constructor<>()),

		class_<vec<3> >("vec3")
		.def(constructor<>())
		.def(constructor<scalar, scalar, scalar>())
		.def(const_self + const_self)
		.def(const_self - const_self)
		.def(const_self * other<scalar>())
		.def("length", &vec<3>::length)
		.def("length_sq", &vec<3>::length_sq),

		class_<vec<4> >("vec4")
		.def(constructor<>()),

		def("dot_product", (scalar (*)(const vec<2> &,const vec<2> &)) dot_product),
		def("dot_product", (scalar (*)(const vec<3> &,const vec<3> &)) dot_product),
		def("dot_product", (scalar (*)(const vec<4> &,const vec<4> &)) dot_product)
	];
}
