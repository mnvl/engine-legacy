#pragma once

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
	enum axis_type {
		AXIS_X		= 0,
		AXIS_Y		= 1,
		AXIS_Z		= 2
	};

	enum parity_type {
		PARITY_EVEN	= 0,
		PARITY_ODD	= 1
	};

	enum repetition_type {
		REPETITION_NO	= 0,
		REPETITION_YES  = 1
	};

	enum frame_type {
		FRAME_STATIC	= 0,
                FRAME_ROTATED	= 1
	};

	typedef unsigned order_type;

        #if defined(_MSC_VER)
        template<unsigned axis, unsigned parity, unsigned rep, unsigned frame>
        #else
	template<axis_type axis, parity_type parity, repetition_type rep, frame_type frame>
        #endif
	struct static_order {
		static order_type const result = (order_type(axis)	<< 3)
				               | (order_type(parity)	<< 2)
					       | (order_type(rep)	<< 1)
					       |  order_type(frame);
	};

	static order_type const ORDER_XYZs    = static_order<AXIS_X,PARITY_EVEN,REPETITION_NO,FRAME_STATIC>::result;
	static order_type const ORDER_XYXs    = static_order<AXIS_X,PARITY_EVEN,REPETITION_YES,FRAME_STATIC>::result;
	static order_type const ORDER_XZYs    = static_order<AXIS_X,PARITY_ODD,REPETITION_NO,FRAME_STATIC>::result;
	static order_type const ORDER_XZXs    = static_order<AXIS_X,PARITY_ODD,REPETITION_YES,FRAME_STATIC>::result;
	static order_type const ORDER_YZXs    = static_order<AXIS_Y,PARITY_EVEN,REPETITION_NO,FRAME_STATIC>::result;
	static order_type const ORDER_YZYs    = static_order<AXIS_Y,PARITY_EVEN,REPETITION_YES,FRAME_STATIC>::result;
	static order_type const ORDER_YXZs    = static_order<AXIS_Y,PARITY_ODD,REPETITION_NO,FRAME_STATIC>::result;
	static order_type const ORDER_YXYs    = static_order<AXIS_Y,PARITY_ODD,REPETITION_YES,FRAME_STATIC>::result;
	static order_type const ORDER_ZXYs    = static_order<AXIS_Z,PARITY_EVEN,REPETITION_NO,FRAME_STATIC>::result;
	static order_type const ORDER_ZXZs    = static_order<AXIS_Z,PARITY_EVEN,REPETITION_YES,FRAME_STATIC>::result;
	static order_type const ORDER_ZYXs    = static_order<AXIS_Z,PARITY_ODD,REPETITION_NO,FRAME_STATIC>::result;
	static order_type const ORDER_ZYZs    = static_order<AXIS_Z,PARITY_ODD,REPETITION_YES,FRAME_STATIC>::result;

	static order_type const ORDER_ZYXr    = static_order<AXIS_X,PARITY_EVEN,REPETITION_NO,FRAME_ROTATED>::result;
	static order_type const ORDER_XYXr    = static_order<AXIS_X,PARITY_EVEN,REPETITION_YES,FRAME_ROTATED>::result;
	static order_type const ORDER_YZXr    = static_order<AXIS_X,PARITY_ODD,REPETITION_NO,FRAME_ROTATED>::result;
	static order_type const ORDER_XZXr    = static_order<AXIS_X,PARITY_ODD,REPETITION_YES,FRAME_ROTATED>::result;
	static order_type const ORDER_XZYr    = static_order<AXIS_Y,PARITY_EVEN,REPETITION_NO,FRAME_ROTATED>::result;
	static order_type const ORDER_YZYr    = static_order<AXIS_Y,PARITY_EVEN,REPETITION_YES,FRAME_ROTATED>::result;
	static order_type const ORDER_ZXYr    = static_order<AXIS_Y,PARITY_ODD,REPETITION_NO,FRAME_ROTATED>::result;
	static order_type const ORDER_YXYr    = static_order<AXIS_Y,PARITY_ODD,REPETITION_YES,FRAME_ROTATED>::result;
	static order_type const ORDER_YXZr    = static_order<AXIS_Z,PARITY_EVEN,REPETITION_NO,FRAME_ROTATED>::result;
	static order_type const ORDER_ZXZr    = static_order<AXIS_Z,PARITY_EVEN,REPETITION_YES,FRAME_ROTATED>::result;
	static order_type const ORDER_XYZr    = static_order<AXIS_Z,PARITY_ODD,REPETITION_NO,FRAME_ROTATED>::result;
	static order_type const ORDER_ZYZr    = static_order<AXIS_Z,PARITY_ODD,REPETITION_YES,FRAME_ROTATED>::result;

	typedef T scalar_type;
	typedef math::quaternion<scalar_type> quaternion_type;	
	typedef matrix<3,3,scalar_type> matrix_type;

	scalar_type x, y, z;
	order_type order;

	static frame_type get_frame(order_type order) {
		return frame_type(order & 1);
        }

	static repetition_type get_repetition(order_type order) {
		return repetition_type((order >> 1) & 1);
	}

	static parity_type get_parity(order_type order) {
		return parity_type((order >> 2) & 1);
	}

	static axis_type get_axis_i(order_type order) {
		return (axis_type) safe[(unsigned(order) >> 3) & 3];
	}

	static axis_type get_axis_j(order_type order) {
		return (axis_type) next[get_axis_i(order) + (get_parity(order) == PARITY_ODD)];
	}

	static axis_type get_axis_k(order_type order) {
		return (axis_type) next[get_axis_i(order) + (get_parity(order) == PARITY_EVEN)];
	}

	static axis_type get_axis_h(order_type order) {
		return (axis_type) (get_repetition(order) == REPETITION_NO) ? get_axis_k(order) : get_axis_i(order);
	}

	static void unpack_order(order_type order, frame_type &frame, repetition_type &rep,
		parity_type &parity,axis_type &i, axis_type &j, axis_type &k, axis_type &h)
	{
		frame = get_frame(order);
		rep = get_repetition(order);
		parity = get_parity(order);
		i = get_axis_i(order);
		j = get_axis_j(order);
		k = get_axis_k(order);
		h = get_axis_h(order);
	}

	void load(matrix_type const &M, order_type ord);
	void load(quaternion_type const &Q, order_type ord);

	void save(matrix_type &M) const;
	void save(quaternion_type &Q) const;

private:
	static char const safe[], next[];
};

namespace math {
	template<class Char,class Traits,class Scalar> std::basic_ostream<Char, Traits> &
	operator <<(std::basic_ostream<Char, Traits> &os, euler_angles<Scalar> const &ea) {
		return os<<ea.x<<' '<<ea.y<<' '<<ea.z<<' '<<ea.order;
	}

	template<class Char,class Traits,class Scalar> std::basic_istream<Char, Traits> &
	operator >>(std::basic_istream<Char, Traits> &is, euler_angles<Scalar> const &ea) {
		return is>>ea.x>>ea.y>>ea.z>>ea.order;
	}
}

