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

#include <math/scalar.h>
#include <math/vec.h>
#include <math/quaternion.h>
#include <math/matrix.h>

namespace math {
	template<class T=scalar> class euler_angles;
}

#if defined(_WIN32) && defined(PARITY_EVEN)
#undef PARITY_EVEN
#endif

#if defined(_WIN32) && defined(PARITY_ODD)
#undef PARITY_ODD
#endif

template<class T>
class math::euler_angles {
public:
	enum axis_t {
		AXIS_X		= 0,
		AXIS_Y		= 1,
		AXIS_Z		= 2
	};

	enum pARITY_t {
		PARITY_EVEN	= 0,
		PARITY_ODD	= 1
	};

	enum repetition_t {
		REPETITION_NO	= 0,
		REPETITION_YES  = 1
	};

	enum frame_t {
		FRAME_STATIC	= 0,
        FRAME_ROTATED	= 1
	};

	typedef unsigned order_t;

        #if defined(_MSC_VER)
        template<unsigned axis, unsigned pARITY, unsigned rep, unsigned frame>
        #else
	template<axis_t axis, pARITY_t pARITY, repetition_t rep, frame_t frame>
        #endif
	struct static_order {
		static order_t const result = (order_t(axis)	<< 3)
									   | (order_t(pARITY)<< 2)
									   | (order_t(rep)	<< 1)
									   |  order_t(frame);
	};

	static order_t const ORDER_XYZs    = static_order<AXIS_X,PARITY_EVEN,REPETITION_NO,FRAME_STATIC>::result;
	static order_t const ORDER_XYXs    = static_order<AXIS_X,PARITY_EVEN,REPETITION_YES,FRAME_STATIC>::result;
	static order_t const ORDER_XZYs    = static_order<AXIS_X,PARITY_ODD,REPETITION_NO,FRAME_STATIC>::result;
	static order_t const ORDER_XZXs    = static_order<AXIS_X,PARITY_ODD,REPETITION_YES,FRAME_STATIC>::result;
	static order_t const ORDER_YZXs    = static_order<AXIS_Y,PARITY_EVEN,REPETITION_NO,FRAME_STATIC>::result;
	static order_t const ORDER_YZYs    = static_order<AXIS_Y,PARITY_EVEN,REPETITION_YES,FRAME_STATIC>::result;
	static order_t const ORDER_YXZs    = static_order<AXIS_Y,PARITY_ODD,REPETITION_NO,FRAME_STATIC>::result;
	static order_t const ORDER_YXYs    = static_order<AXIS_Y,PARITY_ODD,REPETITION_YES,FRAME_STATIC>::result;
	static order_t const ORDER_ZXYs    = static_order<AXIS_Z,PARITY_EVEN,REPETITION_NO,FRAME_STATIC>::result;
	static order_t const ORDER_ZXZs    = static_order<AXIS_Z,PARITY_EVEN,REPETITION_YES,FRAME_STATIC>::result;
	static order_t const ORDER_ZYXs    = static_order<AXIS_Z,PARITY_ODD,REPETITION_NO,FRAME_STATIC>::result;
	static order_t const ORDER_ZYZs    = static_order<AXIS_Z,PARITY_ODD,REPETITION_YES,FRAME_STATIC>::result;

	static order_t const ORDER_ZYXr    = static_order<AXIS_X,PARITY_EVEN,REPETITION_NO,FRAME_ROTATED>::result;
	static order_t const ORDER_XYXr    = static_order<AXIS_X,PARITY_EVEN,REPETITION_YES,FRAME_ROTATED>::result;
	static order_t const ORDER_YZXr    = static_order<AXIS_X,PARITY_ODD,REPETITION_NO,FRAME_ROTATED>::result;
	static order_t const ORDER_XZXr    = static_order<AXIS_X,PARITY_ODD,REPETITION_YES,FRAME_ROTATED>::result;
	static order_t const ORDER_XZYr    = static_order<AXIS_Y,PARITY_EVEN,REPETITION_NO,FRAME_ROTATED>::result;
	static order_t const ORDER_YZYr    = static_order<AXIS_Y,PARITY_EVEN,REPETITION_YES,FRAME_ROTATED>::result;
	static order_t const ORDER_ZXYr    = static_order<AXIS_Y,PARITY_ODD,REPETITION_NO,FRAME_ROTATED>::result;
	static order_t const ORDER_YXYr    = static_order<AXIS_Y,PARITY_ODD,REPETITION_YES,FRAME_ROTATED>::result;
	static order_t const ORDER_YXZr    = static_order<AXIS_Z,PARITY_EVEN,REPETITION_NO,FRAME_ROTATED>::result;
	static order_t const ORDER_ZXZr    = static_order<AXIS_Z,PARITY_EVEN,REPETITION_YES,FRAME_ROTATED>::result;
	static order_t const ORDER_XYZr    = static_order<AXIS_Z,PARITY_ODD,REPETITION_NO,FRAME_ROTATED>::result;
	static order_t const ORDER_ZYZr    = static_order<AXIS_Z,PARITY_ODD,REPETITION_YES,FRAME_ROTATED>::result;

	typedef T scalar_t;
	typedef math::quaternion<scalar_t> quaternion_t;	
	typedef matrix<3,3,scalar_t> matrix_t;

	scalar_t x, y, z;
	order_t order;

	static frame_t get_frame(order_t order) {
		return frame_t(order & 1);
        }

	static repetition_t get_repetition(order_t order) {
		return repetition_t((order >> 1) & 1);
	}

	static pARITY_t get_pARITY(order_t order) {
		return pARITY_t((order >> 2) & 1);
	}

	static axis_t get_axis_i(order_t order) {
		return (axis_t) safe[(unsigned(order) >> 3) & 3];
	}

	static axis_t get_axis_j(order_t order) {
		return (axis_t) next[get_axis_i(order) + (get_pARITY(order) == PARITY_ODD)];
	}

	static axis_t get_axis_k(order_t order) {
		return (axis_t) next[get_axis_i(order) + (get_pARITY(order) == PARITY_EVEN)];
	}

	static axis_t get_axis_h(order_t order) {
		return (axis_t) (get_repetition(order) == REPETITION_NO) ? get_axis_k(order) : get_axis_i(order);
	}

	static void unpack_order(order_t order, frame_t &frame, repetition_t &rep,
		pARITY_t &pARITY,axis_t &i, axis_t &j, axis_t &k, axis_t &h)
	{
		frame = get_frame(order);
		rep = get_repetition(order);
		pARITY = get_pARITY(order);
		i = get_axis_i(order);
		j = get_axis_j(order);
		k = get_axis_k(order);
		h = get_axis_h(order);
	}

	void load(matrix_t const &M, order_t ord);
	void load(quaternion_t const &Q, order_t ord);

	void save(matrix_t &M) const;
	void save(quaternion_t &Q) const;

private:
	static char const safe[], next[];
};

