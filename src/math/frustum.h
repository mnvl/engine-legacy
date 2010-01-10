#pragma once
//-----------------------------------------------------------------------------
// Copyright (c) 2004 - 2010 Manvel Avetisian.
// All Rights Reserved.
//
// This source code contains unpublished, confidential and proprietary
// information of Manvel Avetisian and may not be disclosed to third parties
// or copied or duplicated in any form, in whole or in part, without the
// prior written consent of Manvel Avetisian.
//-----------------------------------------------------------------------------

#include <math/vec.h>
#include <math/matrix.h>
#include <math/aabb.h>
#include <math/triangle.h>

namespace math {
	template<class T=float>
	class frustum;
}

template<class T>
class math::frustum {
public:
	// типы и константы
	typedef T scalar_t;
	typedef size_t size_t;
	static const size_t ARITY=3;
	typedef vec<ARITY,scalar_t> vec_t;
	typedef matrix<ARITY,ARITY,scalar_t> matrix_t;
	typedef line<ARITY,scalar_t> edge_t;
	typedef aabb<ARITY,scalar_t> aabb_t;
	typedef triangle<ARITY,scalar_t> triangle_t;
	typedef frustum<scalar_t> frustum_t;

	// данные
	vec_t origin,bisector;
	scalar_t bisector_length;
	triangle_t top,bottom,left,right;

	// конструкторы
	void construct(const vec_t &o,const vec_t &b,scalar_t fovx,scalar_t fovy) {
		origin=o;
		bisector=b;

		fovx/=2;
		fovy/=2;

		vec_t A,B,C,D;
		rotate(A,bisector,origin,-fovx,-fovy,(scalar_t)0);
		rotate(B,bisector,origin, fovx,-fovy,(scalar_t)0);
		rotate(C,bisector,origin, fovx, fovy,(scalar_t)0);
		rotate(D,bisector,origin,-fovx, fovy,(scalar_t)0);

		bisector-=origin;
		bisector_length=bisector.length();

		A*=bisector_length/A.z;
		B*=bisector_length/B.z;
		C*=bisector_length/C.z;
		D*=bisector_length/D.z;

		top.construct(origin,A,B);
		bottom.construct(origin,D,C);
		left.construct(origin,B,C);
		right.construct(origin,C,A);
	}

	// пересечение с aabb
	bool contains(const vec_t &v) const {
		return top.position(v)>=0 && bottom.position(v)>=0
			&& left.position(v)>=0 && right.position(v)>=0;
	}

	bool contains(const aabb_t &b) const {
		return contains(b.min) || contains(b.max);
	}

	bool intersects(const aabb_t &b) const {
		if(contains(b)) return true;

		vec_t v;
		edge_t e;
		for(int i=0; i<aabb_t::edges_count; i++) {
			b.get_edge(i,e);
			if(top.intersect(e,v) || bottom.intersect(e,v)
				|| left.intersect(e,v) || right.intersect(e,v)) return true;
		}

		return true;
	}
};

