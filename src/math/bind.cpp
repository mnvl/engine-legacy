
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
	bind_interval(L);
	bind_vec(L);
	bind_matrix(L);
	bind_ray(L);
	bind_plane(L);
}

}
