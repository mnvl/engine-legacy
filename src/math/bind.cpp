
#include "interval.h"
#include "vec.h"
#include "matrix.h"
#include "ray.h"
#include "plane.h"
#include "bind.h"

namespace math
{

void bind(lua_State *L)
{
	bind_interval(L_);
	bind_vec(L_);
	bind_matrix(L_);
	bind_ray(L_);
	bind_plane(L_);
}

}
