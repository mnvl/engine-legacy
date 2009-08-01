#pragma once

#include <assert.h>
#include <math/scalar.h>
#include <math/vec.h>
#include <math/sphere.h>
#include <math/aabb.h>
#include <math/ray.h>
#include <math/line.h>
#include <math/triangle.h>
#include <math/matrix.h>

namespace math {
	template<int N,class T=scalar> class obb;
	template<int N,class T> class triangle;

	#pragma pack(push, 1)
	template<class T>
	class obb<2, T> {
	public:
		typedef T scalar_type;
		typedef size_t size_type;
		static const size_type arity = 2;
		typedef vec<arity,scalar_type> vec_type;
		typedef sphere<arity,scalar_type> sphere_type;
		typedef matrix<arity+1,arity+1,scalar_type> matrix_type;
		typedef ray<arity,scalar_type> ray_type;
		typedef line<arity,scalar_type> line_type;
		typedef triangle<arity,scalar_type> triangle_type;
		typedef aabb<arity,scalar_type> aabb_type;
		typedef obb<arity,scalar_type> obb_type;
		static const size_type sides_count = 4;
		static const size_type vertices_count = 4;
	
		vec_type origin, tangent, normal;

		obb() {
		}

		obb(aabb_type const &rhs) {
			origin = rhs.min;
			tangent.set(rhs.max.x - rhs.min.x, 0);
			normal.set(0, rhs.max.y - rhs.min.y);
		}

		void forward_vector(vec_type &v,const vec_type &v0) const {
			v.x = (v0 & tangent) / tangent.length_sq();
			v.y = (v0 & normal)	 / normal.length_sq();
		}

		vec_type forward_vector(const vec_type &v0) const {
			vec_type v;
			forward_vector(v, v0);
			return v;
		}

		void forward_point(vec_type &v,const vec_type &v0) const {
			forward_vector(v, v0 - origin);
		}

		vec_type forward_point(const vec_type &v0) const {
			vec_type v;
			forward_point(v, v0);
			return v;
		}
		
		bool contains(const vec_type &v) const {
			vec_type v1;
			forward_point(v1, v);
			return v1.x>=0 && v1.x<=1
			    && v1.y>=0 && v1.y<=1;
		}

		void transform(matrix_type const &tf) {
			vec_type O = origin, Ox = tangent + origin, Oy = normal + origin;
			mul(origin, O, tf);
			mul(tangent, Ox, tf);
			mul(normal, Oy, tf);
			tangent -= origin;
			normal -= origin;
		}

		vec_type get_vertex(size_type vertex_number) const {
			switch (vertex_number) {
				case 0: return origin;
				case 1: return origin + tangent;
				case 2: return origin + tangent + normal;
				case 3: return origin + normal;
				default: assert(0); return vec_type(0, 0);
			}
		}

		line_type get_side(size_type side_number) const {
			line_type side;
			side.A = get_vertex(side_number);
			side.B = get_vertex((1 + side_number) % sides_count);
			return side;
		}

		vec_type get_normal(size_type side_number) const {
			switch (side_number) {
				case 0: return math::normalize(-normal);
				case 1: return math::normalize(tangent);
				case 2: return math::normalize(normal);
				case 3: return math::normalize(-tangent);
				default: assert(0); return vec_type(0, 0);
			}
		}

		aabb_type get_aabb() const {
			aabb_type a(origin);
			a.extend(origin + tangent);
			a.extend(origin + normal);
			a.extend(origin + tangent + normal);
			return a;
		}
	};
	#pragma pack(pop)

	#pragma pack(push, 1)
	template<class T>
	class obb<3,T> {
	public:
		typedef T scalar_type;
		typedef size_t size_type;
		static const size_type arity = 3;
		typedef vec<arity,scalar_type> vec_type;
		typedef sphere<arity,scalar_type> sphere_type;
		typedef matrix<arity+1,arity+1,scalar_type> matrix_type;
		typedef ray<arity,scalar_type> ray_type;
		typedef triangle<arity,scalar_type> triangle_type;
		typedef aabb<arity,scalar_type> aabb_type;
		typedef obb<arity,scalar_type> obb_type;
		static const size_type sides_count = 6;
		static const size_type vertices_count = 8;
		static const size_type segments_count = 4;
	
		vec_type origin, tangent, normal, binormal;
	
		obb() {
		}
	
		obb(const aabb_type &a) {
			operator =(a);
		}
	
		// сконструировать obb из aabb
		obb_type &operator =(const aabb_type &a) {
			origin = a.min;
			tangent.set(a.max.x - a.min.x, 0, 0);
			normal.set(0, a.max.y - a.min.y, 0);
			binormal.set(0, 0, a.max.z - a.min.z);
			return *this;
		}
	
		// получить объем
		vec_type get_centre() const {
			return origin + (tangent + normal + binormal) / scalar_type(2);
		}
	
		scalar_type get_volume() const {
			return tangent.length() * normal.length() * binormal.length();
		}
	
		// получить aabb
		void get_aabb(aabb_type &loc) const {
			loc.min = loc.max = origin;
			loc.extend(origin + tangent);
			loc.extend(origin + normal);
			loc.extend(origin + tangent + normal);
			loc.extend(origin + binormal);
			loc.extend(origin + tangent + binormal);
			loc.extend(origin + normal + binormal);
			loc.extend(origin + tangent + normal + binormal);
		}
		
		// трансформировать obb матрицей tf
		void transform(const matrix_type &tf) {
			// получить координаты точек - вершин
			vec_type O = origin,
				 Ox = tangent + origin,
				 Oy = normal + origin,
				 Oz = binormal + origin;
	
			// трансформировать эти координаты
			mul(origin, O, tf);
			mul(tangent, Ox, tf);
			mul(normal, Oy, tf);
			mul(binormal, Oz, tf);
	
			// вычислить значения tangent, normal, binormal относительно origin
			tangent -= origin;
			normal -= origin;
			binormal -= origin;
		}

		// получить координаты вершины
		void get_vertex(size_type vertex_number, vec_type &position) const {
			position = origin +	scalar_type(vertex_number & 1) * tangent;
			if (vertex_number & 2) position += normal;
			if (vertex_number & 4) position += binormal;
		}
	
		// получить obb сегмента
		void get_segment(size_type segment_number, obb_type &segment_obb) {
			segment_obb.tangent = tangent / scalar_type(2);
			segment_obb.normal = normal / scalar_type(2);
			segment_obb.binormal = binormal	/ scalar_type(2);
			segment_obb.origin = origin;
	
			if(segment_number & 1) segment_obb.origin += segment_obb.tangent;
			if(segment_number & 2) segment_obb.origin += segment_obb.normal;
			if(segment_number & 4) segment_obb.origin += segment_obb.binormal;
		}
	
		// получить координаты вектора в локальной системе координат с
		// базисом (tangent,normal,binormal)
		void forward_vector(vec_type &v,const vec_type &v0) const {
			v.x = (v0 & tangent)	/ tangent.length_sq();
			v.y = (v0 & normal)	/ normal.length_sq();
			v.z = (v0 & binormal)	/ binormal.length_sq();
		}
	
		// получить координаты точки в локальной системе координат
		void forward_point(vec_type &v,const vec_type &v0) const {
			forward_vector(v, v0 - origin);
		}
	
		// перенести прямую в локальную систему координат
		void forward_ray(ray_type &r,const ray_type &r0) const {
			forward_point(r.origin,r0.origin);
			forward_vector(r.direction,r0.direction);
		}
	
		// проверить принадлежность точки
		bool contains(const vec_type &v) const {
			vec_type v1;
			forward_point(v1, v);
			return v1.x>=0 && v1.x<=1
			    && v1.y>=0 && v1.y<=1
			    && v1.z>=0 && v1.z<=1;
		}
	
		// проверить принадлежность треугольника
		bool contains(const triangle_type &tri) const {
			return contains(tri.A) && contains(tri.B) && contains(tri.C);
		}
	
		// проверить пересечение с лучом
		 bool trace(const ray_type &r, scalar_type t_min = 1,
				 scalar_type t_max = 1) const;
	       	
		// проверить на содержание obb
		bool contains(const obb_type &r) const;

		// пересекаются ли obb и треугольник?
		bool intersects(const triangle_type &tri) const {
			return !not_intersects(tri);
		}
		
		// пересекаются ли два obb?
		bool intersects(const obb_type &box) const {
			return !not_intersects(box) && !box.not_intersects(*this);
		}

		// столкнутся ли два движущихся obb в интервале времени [t_min, t_max]?
		bool collides(triangle_type const &tri, const vec_type &vel, scalar_type t_min = 0,
			scalar_type t_max = 1) const
		{
			return !not_collides(tri, vel, t_min, t_max);
		}

		// столкнутся ли два движущихся obb в интервале времени [t_min, t_max]?
		bool collides(obb_type const &box, const vec_type &vel, scalar_type t_min = 0,
			scalar_type t_max = 1) const
		{
			return !not_collides(box, vel, t_min, t_max)
			    && !box.not_collides(*this, -vel, t_min, t_max);
		}

	private:
		// проверить на не-пересечение с треугольником
		bool not_intersects(const triangle_type &r) const;
	
		// проверить на одностороннее не-пересечение с obb
		bool not_intersects(const obb_type &r) const;

		// проверить на одностороннее не-столкновение с треугольником
		// r -- данный треугольник
		// vel -- скорость box относительно настоящего obb
		// t_min, t_max -- интервал времени, внутри которого следует искать столкновение
		bool not_collides(const triangle_type &r, const vec_type &vel, scalar_type t_min = 0,
			scalar_type t_max = 1) const;

		// проверить на одностороннее не-столкновение с obb
		// r -- данный obb
		// vel -- скорость box относительно настоящего obb
		// t_min, t_max -- интервал времени, внутри которого следует искать столкновение
		bool not_collides(const obb_type &r, const vec_type &vel, scalar_type t_min = 0,
			scalar_type t_max = 1) const;
	};
	#pragma pack(pop)
}

