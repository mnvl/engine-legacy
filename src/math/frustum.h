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
	typedef T scalar_type;
	typedef size_t size_type;
	static const size_type arity=3;
	typedef vec<arity,scalar_type> vec_type;
	typedef matrix<arity,arity,scalar_type> matrix_type;
	typedef line<arity,scalar_type> edge_type;
	typedef aabb<arity,scalar_type> aabb_type;
	typedef triangle<arity,scalar_type> triangle_t;
	typedef frustum<scalar_type> frustum_type;

	// данные
	vec_type origin,bisector;
	scalar_type bisector_length;
	triangle_t top,bottom,left,right;

	// конструкторы
	void construct(const vec_type &o,const vec_type &b,scalar_type fovx,scalar_type fovy) {
		origin=o;
		bisector=b;

		fovx/=2;
		fovy/=2;

		vec_type A,B,C,D;
		rotate(A,bisector,origin,-fovx,-fovy,(scalar_type)0);
		rotate(B,bisector,origin, fovx,-fovy,(scalar_type)0);
		rotate(C,bisector,origin, fovx, fovy,(scalar_type)0);
		rotate(D,bisector,origin,-fovx, fovy,(scalar_type)0);

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
	bool contains(const vec_type &v) const {
		return top.position(v)>=0 && bottom.position(v)>=0
			&& left.position(v)>=0 && right.position(v)>=0;
	}

	bool contains(const aabb_type &b) const {
		return contains(b.min) || contains(b.max);
	}

	bool intersects(const aabb_type &b) const {
		if(contains(b)) return true;

		vec_type v;
		edge_type e;
		for(int i=0; i<aabb_type::edges_count; i++) {
			b.get_edge(i,e);
			if(top.intersect(e,v) || bottom.intersect(e,v)
				|| left.intersect(e,v) || right.intersect(e,v)) return true;
		}

		return true;
	}
};

