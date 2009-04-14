#pragma once

#include <engine/math/scalar.h>
#include <engine/math/vec.h>
#include <engine/math/ray.h>
#include <engine/math/line.h>

namespace math {
	template<int N,class T=scalar> class aabb;

	template<int N,class T> class triangle;
}

template<int N,class T>
class math::aabb {
public:
	// типы и константы
	typedef T scalar_type;
	typedef std::numeric_limits<scalar_type> limits_type;
	typedef size_t size_type;
	static const size_type arity = N;
	typedef vec<arity,scalar_type> vec_type;
	static const size_type vertices_count = 1 << arity;
	static const size_type edges_count = vertices_count + (1 << (arity-1));
	typedef line<arity,scalar_type> line_type;
	typedef ray<arity,scalar_type> ray_type;
	typedef triangle<arity, scalar_type> triangle_type;
	typedef aabb<arity,scalar_type> aabb_type;

	// данные
	vec_type min, max;

	// конструкторы
	aabb() {
	}

	aabb(const vec_type &centre):
		min(centre),
		max(centre)
	{
	}

	aabb(const vec_type &mi,const vec_type &ma):
		min(mi),
		max(ma)
	{
	}

	aabb(const vec_type &centre, math::scalar radius):
		min(centre.x - radius/2, centre.y - radius/2, centre.z - radius/2),
		max(centre.x + radius/2, centre.y + radius/2, centre.z + radius/2)
       	{
	}

	// операции
	void null() {
		for(size_type n=0; n<arity; n++) {
			min.i[n] = limits_type::max();
			max.i[n] = limits_type::min();
		}
	}

	void forward_vector(vec_type &v,const vec_type &v0) const {
		v.x = v0.x / (max.x - min.x);
		v.y = v0.y / (max.y - min.y);
		v.z = v0.z / (max.z - min.z);
	}

	// получить координаты точки в локальной системе координат
	void forward_point(vec_type &v,const vec_type &v0) const {
		forward_vector(v, v0 - min);
	}

	// перенести прямую в локальную систему координат
	void forward_ray(ray_type &r,const ray_type &r0) const {
		forward_point(r.origin,r0.origin);
		forward_vector(r.direction,r0.direction);
	}

	void normalize() {
		minmax(min,max);
	}

	vec_type unit() const {
		return max-min;
	}

	vec_type centre() const {
		return min+unit()/2;
	}

	void extend(const vec_type &v) {
		for(size_type n=0; n<arity; n++) {
			if(min.i[n] > v.i[n]) min.i[n] = v.i[n];
			if(max.i[n] < v.i[n]) max.i[n] = v.i[n];
		}
	}

	void extend(const aabb_type &b) {
		extend(b.min);
		extend(b.max);
	}

	// пересечения
	bool contains(const vec_type &right) const {
		return min<right && right<max;
	}

	bool contains(const aabb_type &right) const {
		return contains(right.min) && contains(right.max);
	}

	bool contains(const triangle_type &right) const {
		return contains(right.A) && contains(right.B) && contains(right.C);
	}

	bool intersects(const aabb_type &right) const {
		return right.max>min && max>right.min;
	}

	bool trace(const ray_type &r, scalar_type t_min = 0, scalar_type t_max = 1) const {
		// перенести луч в локальную систему координат
		ray_type r1;
		forward_ray(r1, r);

		// пересечение прямой r1 с плоскостьями x=0 и x=1
		if(abs(r1.direction.x) > limits_type::epsilon()) {
			scalar_type dx = 1 / r1.direction.x;
		
			scalar_type t = -r1.origin.x * dx;
			vec_type v;
			if(t >= t_min && t <= t_max) {
				v = r1.origin + r1.direction * t;
				if(v.y>=0 && v.y<=1 && v.z>=0 && v.z<=1) return true;
			}

			t = dx - t;
			if(t >= t_min && t <= t_max) {
				v += r1.direction * dx;
				if(v.y>=0 && v.y<=1 && v.z>=0 && v.z<=1) return true;
			}
		}

		// пересечение прямой r1 с плоскостьями y=0 и y=1
		if(abs(r1.direction.y) > limits_type::epsilon()) {
			scalar_type dy = 1 / r1.direction.y;
			
			scalar_type t = -r1.origin.y * dy;
			vec_type v;
			if(t >= t_min && t <= t_max) {
				v = r1.origin + r1.direction * t;
				if(v.x>=0 && v.x<=1 && v.z>=0 && v.z<=1) return true;
			}

			t = dy - t;
			if(t >= t_min && t <= t_max) {
				v += r1.direction * dy;
				if(v.x>=0 && v.x<=1 && v.z>=0 && v.z<=1) return true;
			}
		}

		// пересечение прямой r1 с плоскостьями z=0 и z=1
		if(abs(r1.direction.z) > limits_type::epsilon()) {
			scalar_type dz = 1 / r1.direction.z;

			scalar_type t = -r1.origin.z * dz;
			vec_type v;
			if(t >= t_min && t <= t_max) {
				v = r1.origin + r1.direction * t;
				if(v.x>=0 && v.x<=1 && v.y>=0 && v.y<=1) return true;
			}

			t = dz - t;
			if(t >= t_min && t <= t_max) {
				v += r1.direction * dz;
				if(v.x>=0 && v.x<=1 && v.y>=0 && v.y<=1) return true;
			}
		}

		return false;
	}

	// получение вершин, ребер, сегментов
	void get_vertex(int n,vec_type &v) const {
		v.select(n,min,max);
	}

	void get_edge(int n,line_type &e) const {
		vec_type v1,v2;
		if(n<vertices_count) {
			get_vertex(n,v1);
			get_vertex((n+1)%vertices_count,v2);
		} else {
			get_vertex(n-vertices_count,v1);
			get_vertex(n-(1<<arity-1),v2);
		}
		e.construct(v1,v2-v1);
	}

	void get_segment(size_type segment_number, aabb_type &segment_aabb) {
		vec_type half_unit = (max - min) / scalar_type(2);

		vec_type half_unit_projection = half_unit;
		half_unit_projection.projection(segment_number);

		segment_aabb.min = min + half_unit_projection;
		segment_aabb.max = segment_aabb.min + half_unit;
	}
};
