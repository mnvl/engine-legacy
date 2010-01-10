//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

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
		typedef T scalar_t;
		typedef size_t size_t;
		static const size_t ARITY = 2;
		typedef vec<ARITY,scalar_t> vec_t;
		typedef sphere<ARITY,scalar_t> sphere_t;
		typedef matrix<ARITY+1,ARITY+1,scalar_t> matrix_t;
		typedef ray<ARITY,scalar_t> ray_t;
		typedef line<ARITY,scalar_t> line_t;
		typedef triangle<ARITY,scalar_t> triangle_t;
		typedef aabb<ARITY,scalar_t> aabb_t;
		typedef obb<ARITY,scalar_t> obb_t;
		static const size_t sides_count = 4;
		static const size_t vertices_count = 4;
	
		vec_t origin, tangent, normal;

		obb() {
		}

		obb(aabb_t const &rhs) {
			origin = rhs.min;
			tangent.set(rhs.max.x - rhs.min.x, 0);
			normal.set(0, rhs.max.y - rhs.min.y);
		}

		void forward_vector(vec_t &v,const vec_t &v0) const {
			v.x = (v0 & tangent) / tangent.length_sq();
			v.y = (v0 & normal)	 / normal.length_sq();
		}

		vec_t forward_vector(const vec_t &v0) const {
			vec_t v;
			forward_vector(v, v0);
			return v;
		}

		void forward_point(vec_t &v,const vec_t &v0) const {
			forward_vector(v, v0 - origin);
		}

		vec_t forward_point(const vec_t &v0) const {
			vec_t v;
			forward_point(v, v0);
			return v;
		}
		
		bool contains(const vec_t &v) const {
			vec_t v1;
			forward_point(v1, v);
			return v1.x>=0 && v1.x<=1
			    && v1.y>=0 && v1.y<=1;
		}

		void transform(matrix_t const &tf) {
			vec_t O = origin, Ox = tangent + origin, Oy = normal + origin;
			mul(origin, O, tf);
			mul(tangent, Ox, tf);
			mul(normal, Oy, tf);
			tangent -= origin;
			normal -= origin;
		}

		vec_t get_vertex(size_t vertex_number) const {
			switch (vertex_number) {
				case 0: return origin;
				case 1: return origin + tangent;
				case 2: return origin + tangent + normal;
				case 3: return origin + normal;
				default: assert(0); return vec_t(0, 0);
			}
		}

		line_t get_side(size_t side_number) const {
			line_t side;
			side.A = get_vertex(side_number);
			side.B = get_vertex((1 + side_number) % sides_count);
			return side;
		}

		vec_t get_normal(size_t side_number) const {
			switch (side_number) {
				case 0: return math::normalize(-normal);
				case 1: return math::normalize(tangent);
				case 2: return math::normalize(normal);
				case 3: return math::normalize(-tangent);
				default: assert(0); return vec_t(0, 0);
			}
		}

		aabb_t get_aabb() const {
			aabb_t a(origin);
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
		typedef T scalar_t;
		typedef size_t size_t;
		static const size_t ARITY = 3;
		typedef vec<ARITY,scalar_t> vec_t;
		typedef sphere<ARITY,scalar_t> sphere_t;
		typedef matrix<ARITY+1,ARITY+1,scalar_t> matrix_t;
		typedef ray<ARITY,scalar_t> ray_t;
		typedef triangle<ARITY,scalar_t> triangle_t;
		typedef aabb<ARITY,scalar_t> aabb_t;
		typedef obb<ARITY,scalar_t> obb_t;
		static const size_t sides_count = 6;
		static const size_t vertices_count = 8;
		static const size_t segments_count = 4;
	
		vec_t origin, tangent, normal, binormal;
	
		obb() {
		}
	
		obb(const aabb_t &a) {
			operator =(a);
		}
	
		// сконструировать obb из aabb
		obb_t &operator =(const aabb_t &a) {
			origin = a.min;
			tangent.set(a.max.x - a.min.x, 0, 0);
			normal.set(0, a.max.y - a.min.y, 0);
			binormal.set(0, 0, a.max.z - a.min.z);
			return *this;
		}
	
		// получить объем
		vec_t get_centre() const {
			return origin + (tangent + normal + binormal) / scalar_t(2);
		}
	
		scalar_t get_volume() const {
			return tangent.length() * normal.length() * binormal.length();
		}
	
		// получить aabb
		void get_aabb(aabb_t &loc) const {
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
		void transform(const matrix_t &tf) {
			// получить координаты точек - вершин
			vec_t O = origin,
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
		void get_vertex(size_t vertex_number, vec_t &position) const {
			position = origin +	scalar_t(vertex_number & 1) * tangent;
			if (vertex_number & 2) position += normal;
			if (vertex_number & 4) position += binormal;
		}
	
		// получить obb сегмента
		void get_segment(size_t segment_number, obb_t &segment_obb) {
			segment_obb.tangent = tangent / scalar_t(2);
			segment_obb.normal = normal / scalar_t(2);
			segment_obb.binormal = binormal	/ scalar_t(2);
			segment_obb.origin = origin;
	
			if(segment_number & 1) segment_obb.origin += segment_obb.tangent;
			if(segment_number & 2) segment_obb.origin += segment_obb.normal;
			if(segment_number & 4) segment_obb.origin += segment_obb.binormal;
		}
	
		// получить координаты вектора в локальной системе координат с
		// базисом (tangent,normal,binormal)
		void forward_vector(vec_t &v,const vec_t &v0) const {
			v.x = (v0 & tangent)	/ tangent.length_sq();
			v.y = (v0 & normal)	/ normal.length_sq();
			v.z = (v0 & binormal)	/ binormal.length_sq();
		}
	
		// получить координаты точки в локальной системе координат
		void forward_point(vec_t &v,const vec_t &v0) const {
			forward_vector(v, v0 - origin);
		}
	
		// перенести прямую в локальную систему координат
		void forward_ray(ray_t &r,const ray_t &r0) const {
			forward_point(r.r0,r0.r0);
			forward_vector(r.a,r0.a);
		}
	
		// проверить принадлежность точки
		bool contains(const vec_t &v) const {
			vec_t v1;
			forward_point(v1, v);
			return v1.x>=0 && v1.x<=1
			    && v1.y>=0 && v1.y<=1
			    && v1.z>=0 && v1.z<=1;
		}
	
		// проверить принадлежность треугольника
		bool contains(const triangle_t &tri) const {
			return contains(tri.A) && contains(tri.B) && contains(tri.C);
		}
	
		// проверить пересечение с лучом
		 bool trace(const ray_t &r, scalar_t t_min = 1,
				 scalar_t t_max = 1) const;
	       	
		// проверить на содержание obb
		bool contains(const obb_t &r) const;

		// пересекаются ли obb и треугольник?
		bool intersects(const triangle_t &tri) const {
			return !not_intersects(tri);
		}
		
		// пересекаются ли два obb?
		bool intersects(const obb_t &box) const {
			return !not_intersects(box) && !box.not_intersects(*this);
		}

		// столкнутся ли два движущихся obb в интервале времени [t_min, t_max]?
		bool collides(triangle_t const &tri, const vec_t &vel, scalar_t t_min = 0,
			scalar_t t_max = 1) const
		{
			return !not_collides(tri, vel, t_min, t_max);
		}

		// столкнутся ли два движущихся obb в интервале времени [t_min, t_max]?
		bool collides(obb_t const &box, const vec_t &vel, scalar_t t_min = 0,
			scalar_t t_max = 1) const
		{
			return !not_collides(box, vel, t_min, t_max)
			    && !box.not_collides(*this, -vel, t_min, t_max);
		}

	private:
		// проверить на не-пересечение с треугольником
		bool not_intersects(const triangle_t &r) const;
	
		// проверить на одностороннее не-пересечение с obb
		bool not_intersects(const obb_t &r) const;

		// проверить на одностороннее не-столкновение с треугольником
		// r -- данный треугольник
		// vel -- скорость box относительно настоящего obb
		// t_min, t_max -- интервал времени, внутри которого следует искать столкновение
		bool not_collides(const triangle_t &r, const vec_t &vel, scalar_t t_min = 0,
			scalar_t t_max = 1) const;

		// проверить на одностороннее не-столкновение с obb
		// r -- данный obb
		// vel -- скорость box относительно настоящего obb
		// t_min, t_max -- интервал времени, внутри которого следует искать столкновение
		bool not_collides(const obb_t &r, const vec_t &vel, scalar_t t_min = 0,
			scalar_t t_max = 1) const;
	};
	#pragma pack(pop)
}

