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

#include "scalar.h"
#include "vec.h"
#include "matrix.h"

namespace math
{

template<int N,int M,class T> class matrix;
template<class T = scalar> class quaternion;

template<class T> inline T
inner_product(const quaternion<T> &q0, const quaternion<T> &q1);

template<class T> inline quaternion<T>
lerp_unit(const quaternion<T> &q1, const quaternion<T> &q2, T t);

template<class T = scalar> class quaternion_lerper;
template<class T = scalar> class quaternion_slerper;

template<class T> inline vec<3,T>
ortho_project_on_sphere(T x , T y);

template<class T> inline quaternion<T>
arcball(const vec<3,T> &from, const vec<3,T> &to);

template<class T> inline quaternion<T>
trackball(const vec<3,T> &from, const vec<3,T> &to);

template<class T> inline void
swing_twist_decomposition(const quaternion<T> &rotation,const vec<3,T> &direction,
	quaternion<T> &swing,quaternion<T> &twist);

template<class T>
class quaternion {
public:
	// типы и константы
	typedef T scalar_t;
	typedef vec<3,T> vec_t;
	typedef matrix<3,3,T> matrix_t;
	typedef quaternion<T> quaternion_t;

	// данные
    scalar_t x,y,z,w;

    quaternion() {
	}

    quaternion(scalar_t x0, scalar_t y0, scalar_t z0, scalar_t w0):
		x(x0),
		y(y0),
		z(z0),
		w(w0)
	{
	}

    void set(scalar_t x0, scalar_t y0, scalar_t z0, scalar_t w0) {
        x = x0;
		y = y0;
		z = z0;
		w = w0;
	}

    void set(const quaternion_t& q) {
       	x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
    }

    void identity() {
       	x = 0;
		y = 0;
		z = 0;
		w = 1;
    }

    void conjugate() {
       	x = -x;
		y = -y;
		z = -z;
    }

    void conjugated(quaternion_t &q) const {
       	q = *this;
		q.conjugate();
    }

    quaternion_t conjugated() const {
       	quaternion_t q;
		conjugated(q);
		return q;
    }

    void scale(scalar_t s) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;
    }

    scalar_t norm() const {
       	return x*x + y*y + z*z + w*w;
    }

    scalar_t magnitude() const {
       	return (scalar_t) sqrt(norm());
    }

	void invert() {
       	scalar_t in = 1 / norm();
       	set(-x*in, -y*in, -z*in, w*in);
    }

	void inverted(quaternion_t &q) {
		q=*this;
		q.invert();
	}

    quaternion_t inverted() const {
		quaternion_t q;
		inverted(q);
		return q;
    }

    /**
	   stabilize quaternion length within 1 - 1/4
	   this operation is a lot faster than normalization
	   and preserve length goes to 0 or infinity
    */
    void stabilize_length() {
		scalar_t cs = (scalar_t)(fabs(x) + fabs(y) + fabs(z) + fabs(w));
       	if(cs > 0.0f)
			set(x/=cs,  y/=cs,  z/=cs,  w/=cs); 
       	else
        	identity();
    }

   	/**
	   scale quaternion that its norm goes to 1 
	   the appearing of 0 magnitude or near is a error,
	   so we can be sure that can divide by magnitude
	*/
   	void normalize() {
		scalar_t m = magnitude();
		if(m < EPSILON) {
			stabilize_length();
			m = magnitude();
		}
		scale(1/m);
	}

	bool equal(const quaternion_t& q){
		return (fabs(q.x - x) < EPSILON) &&
			(fabs(q.y - y) < EPSILON) &&
			(fabs(q.z - z) < EPSILON) &&
			(fabs(q.w - w) < EPSILON);
	}

	//----operators------------------------------------------------
	inline quaternion_t operator +() const {
		return *this;
 	}

	quaternion_t operator -() const {
		return quaternion_t(-x, -y, -z, -w);
	}

	quaternion_t &operator +=(const quaternion_t& q) {
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
	}

	quaternion_t &operator -=(const quaternion& q) {
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;

		return *this;
	}

	quaternion_t &operator *=(scalar_t s) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;

		return *this;
	}

	/**
	   q(cross(v,v') + wv' + w'v, ww' - dot(v,v'))
	*/
	quaternion_t &operator *=(const quaternion& q) {
		quaternion_t p=*this;
		x = p.w*q.x + p.x*q.w + p.y*q.z - p.z*q.y;
		y = p.w*q.y + p.y*q.w + p.z*q.x - p.x*q.z;
		z = p.w*q.z + p.z*q.w + p.x*q.y - p.y*q.x;
		w = p.w*q.w - p.x*q.x - p.y*q.y - p.z*q.z;

		return *this;
	}

    /**
	   return rotation angle from -PI to PI
    */
    scalar_t get_rotation_angle() const {
       	if(w < 0) return 2 * atan2(-sqrt(x*x + y*y + z*z), -w);
       	else return 2 * atan2(sqrt(x*x + y*y + z*z),  w);
    }

    /**
	   create a unit quaternion from axis angle representation
    */
    void rotation(const vec_t &axis, scalar_t angle) {
		if(angle) {
			vec_t v(axis);
			v.normalize();
			scalar_t half_angle = angle * scalar_t(0.5);
			scalar_t sin_a = (scalar_t) sin(half_angle);
			set(v.x*sin_a, v.y*sin_a, v.z*sin_a, cos(half_angle));
		} else {
			identity();
		}
    }

	// построить кватернион поворотов на углы alpha, beta, gamma
	void rotation(scalar_t alpha, scalar_t beta, scalar_t gamma) {
		vec_t axis(alpha, beta, gamma);
		scalar_t angle = axis.length();
		rotation(axis, angle);
	}

	// построить кватернион поворотов на углы rot.x, rot.y, rot.z
	void rotation(const vec_t &rot) {
		scalar_t angle = rot.length();
		rotation(rot, angle);
	}

	// получить кватернион поворота вектора (1,0,0) в вектор dir
	void direction(const vec_t &dir) {
		matrix_t m;
		m.direction(dir);

		set_unit(m);
	}

	// умножить настоящий кватернион на кватернион поворота вектора (1,0,0) в вектор dir
	void direct(const vec_t &dir) {
		quaternion_t q;
		q.direction(dir);

		*this *= dir;
	}

   	/**
	   convert a quaternion to axis angle representation, 
	   preserve the axis direction and angle from -PI to +PI
    */
    void decompose(vec_t &axis, scalar_t &angle)const {
       	scalar_t vl = sqrt(x*x + y*y + z*z);
       	if(vl > EPSILON) {
       		scalar_t ivl = 1 / vl;
      		axis.set(x*ivl, y*ivl, z*ivl);
       		if(w < 0)
           		angle = 2 * atan2(-vl, -w); //-PI,0 
       		else
           		angle = 2 * atan2(vl,  w); //0,PI 
      	} else {
			axis.set(0,0,0);
			angle = 0;
      	}
	}

   	/**
	   create a unit quaternion by rotation matrix
	   martix must contain only rotation (not scale or shear)

	   For numerical stability we find first the greatest component of quaternion
	   and than search others from this one
	*/
	template <int M, int N, class X>
   	void set_unit(matrix<M, N, X> const &m)
   	{
       	scalar_t tr = m.ij[0][0] + m.ij[1][1] + m.ij[2][2];	// trace of martix
       	if (tr > 0) {	// if trace positive than "w" is biggest component
			set(m.ij[1][2] - m.ij[2][1], m.ij[2][0] - m.ij[0][2], m.ij[0][1] - m.ij[1][0], tr+1);
			scale(scalar_t(0.5) / scalar_t(sqrt(w)));	// "w" contain the "norm * 4"
       	} else {
			// Some of vector components is bigger
       		if((m.ij[0][0] > m.ij[1][1]) && (m.ij[0][0] > m.ij[2][2])) {
      			set(1 + m.ij[0][0] - m.ij[1][1] - m.ij[2][2], m.ij[1][0] + m.ij[0][1],
           			m.ij[2][0] + m.ij[0][2], m.ij[1][2] - m.ij[2][1]);
      			scale(scalar_t(0.5) / scalar_t(sqrt(x)));
       		} else {
      			if (m.ij[1][1] > m.ij[2][2]) {
		        	set(m.ij[1][0] + m.ij[0][1], 1 + m.ij[1][1] - m.ij[0][0] - m.ij[2][2],
			        	m.ij[2][1] + m.ij[1][2], m.ij[2][0] - m.ij[0][2]); 
           			scale(scalar_t(0.5) / scalar_t(sqrt(y)));
       			} else {
		        	set(m.ij[2][0] + m.ij[0][2], m.ij[2][1] + m.ij[1][2],
						1 + m.ij[2][2] - m.ij[0][0] - m.ij[1][1],
						m.ij[0][1] - m.ij[1][0]);
					scale(scalar_t(0.5) / scalar_t(sqrt(z)));
				}
			}
		}
	}

   	/**
	   create a nonunit quaternion from rotation matrix 
	   martix must contain only rotation (not scale or shear)
	   the result quaternion length is numerical stable 
    */
	template <int M, int N, class X>
  	void set(matrix<M, N, X> const &m) {
       	scalar_t tr = m.ij[0][0] + m.ij[1][1] + m.ij[2][2]; // trace of martix
       	if (tr > 0){  // if trace positive than "w" is biggest component
			set(m.ij[1][2] - m.ij[2][1], m.ij[2][0] - m.ij[0][2], m.ij[0][1] - m.ij[1][0], tr + 1);
       	} else {
			// Some of vector components is bigger
       		if((m.ij[0][0] > m.ij[1][1]) && (m.ij[0][0] > m.ij[2][2])) {
      			set(1 + m.ij[0][0] - m.ij[1][1] - m.ij[2][2], m.ij[1][0] + m.ij[0][1],
					m.ij[2][0] + m.ij[0][2], m.ij[1][2] - m.ij[2][1]);
			} else {
				if (m.ij[1][1] > m.ij[2][2]) {
					set(m.ij[1][0] + m.ij[0][1], 1 + m.ij[1][1] - m.ij[0][0] - m.ij[2][2],
           				m.ij[2][1] + m.ij[1][2], m.ij[2][0] - m.ij[0][2]); 
				} else {
					set(m.ij[2][0] + m.ij[0][2], m.ij[2][1] + m.ij[1][2],
						1 + m.ij[2][2] - m.ij[0][0] - m.ij[1][1], m.ij[0][1] - m.ij[1][0]);
				}
			}
		}
    }

  	/**
	   shortest arc quaternion rotate one vector to another by shortest path.
	   create rotation from -> to, for any length vectors.
    */
    void shortest_arc(const vec_t &from, const vec_t &to)
    {
       	vec_t c(from*to);
       	set(c.x, c.y, c.z, from & to);
       	normalize();    // if "from" or "to" not unit, normalize quat
       	w += 1;		// reducing angle to halfangle
       	if(w <= EPSILON) // angle close to PI
       	{
     		if((from.z*from.z) > (from.x*from.x))
          		set(0, from.z, - from.y, w); //from*vector3(1,0,0) 
    		else
          		set(from.y, - from.x, 0, w); //from*vector3(0,0,1) 
      	}

      	normalize();
  	}
};

template<class T> T
inner_product(const quaternion<T> &q0, const quaternion<T> &q1) {
    return q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.w*q1.w;
}

template<class T> inline quaternion<T>
operator +(const quaternion<T> &lhs, const quaternion<T> &rhs) {
	quaternion<T> temp = lhs;
	temp += rhs;
	return temp;
}

/**
  perform linear interpolation (approximate rotation interpolation) of two unit quats,
  result quaternion nonunit, its length lay between. sqrt(2)/2  and 1.0
*/
template<class T> inline quaternion<T>
lerp_unit(const quaternion<T> &q1, const quaternion<T> &q2, T t) {
    T inner = inner_product(q1, q2);
    if(inner < 0){
        return q1 - (q2+q1)*t;
    }else{
        return q1 + (q2-q1)*t;
    }
}
/**
  class perform linear interpolation (approximate rotation interpolation),
  result quaternion nonunit, its length lay between. sqrt(2)/2  and 1.0
*/
template<class T>
class quaternion_lerper {
public:
	typedef T scalar_t;
	typedef quaternion<scalar_t> quaternion_t;

private:
    quaternion_t  q1, q2;

public:

    void setup_from_unit(const quaternion_t & p, const quaternion_t& q){
        q1 =  p;
        q2 =  q;
        scalar_t inner = inner_product(q1, q2);
        if(inner < 0) q2 = -q2;
        q2 -= q1;
    }

    void setup(const quaternion_t &p, const quaternion_t &q) {
        q1 =  p;     q1.normalize();
        q2 =  q;     q2.normalize();
        setup_from_unit(q1, q2);
    }

    void interpolate(scalar_t t, quaternion_t & result_q) const {
		result_q = q1 + q2*t;
    }

	quaternion_t interpolate(scalar_t t) const {
		quaternion_t result_q;
		interpolate(t, result_q);
		return result_q;
	}
};
/**
 Perform Spherical Linear Interpolation of the quaternions,
 return unit length quaternion 
*/
template<class T>
class quaternion_slerper
{
public:
	typedef T scalar_t;
	typedef quaternion<T> quaternion_t;

private:
    quaternion_t q1, q2;
    scalar_t omega;

public:
    void setup_from_unit(const quaternion_t &p, const quaternion_t &q){
        q1 = p;	q2 = q;
        scalar_t cos_omega = inner_product(q1, q2);
        if(cos_omega < 0) {
            cos_omega = -cos_omega;
            q2 = -q2;
        }
		if(cos_omega > (1 - EPSILON)) cos_omega = 1 - EPSILON;
        omega = (scalar_t) acos(cos_omega);
        scalar_t inv_sin_omega = (scalar_t)(1/sin(omega));
        q1.scale(inv_sin_omega);
        q2.scale(inv_sin_omega);
    }

    void setup(const quaternion_t &p, const quaternion_t &q){
        q1 = p;
        q1.normalize();
        q2 = q;
        q2.normalize();
        setup_from_unit(q1, q2);
    }

    void interpolate(scalar_t t, quaternion_t &result_q) const {
        result_q = q1*(scalar_t)sin((1 - t)*omega) + q2*(scalar_t)sin(t*omega);
    }

	quaternion_t interpolate(scalar_t t) const {
		quaternion_t result_q;
		interpolate(t, result_q);
		return result_q;
	}
};

/**
 help function for arcball 
 orthogonal projection on sphere of radius 1, 
 standing in (x = 0, y = 0)
*/
template<class T> vec<3,T>
ortho_project_on_sphere(T x , T y)
{
    vec<3,T> p(x, y, 0);
    scalar ls = p.length_sq();
    if (ls >= 1)
        p.norm();
    else
	    p.z = T(sqrt(1 - ls));
    return p;
}

/*
  calculate rotation of arcball user input,
  used to perform object rotation by mouse.
  "from" and "to" the mouse on screen coordinates (with - x,y on screen) 
  in range of -1 to +1
  the arcball radius is 1.0 and it stand in a centre (x = 0, y = 0)
*/
template<class T> quaternion<T>
arcball(const vec<3,T> &from, const vec<3,T> &to)
{
    vec<3,T> p_f = ortho_project_on_sphere(from.x, from.y);
    vec<3,T> p_t = ortho_project_on_sphere(to.x, to.y);
    vec<3,T> c   = cross(p_f, p_t);
    T d   = dot(p_f, p_t);
    return  quaternion<T>(c.x, c.y, c.z, d);
}

/**
 rotate as a half arcball, more realistic manner (like 3dmax).
*/
template<class T> quaternion<T>
trackball(const vec<3,T> &from, const vec<3,T> &to)
{
    quaternion<T> rot;
    rot.shortest_arc(ortho_project_on_sphere(from.x, from.y),
                      ortho_project_on_sphere(to.x, to.y));
    return rot;
}

/**
   Decompose the rotation on to 2 parts.
   1. Twist - rotation around the "direction" vector
   2. Swing - rotation around axis that is perpendicular to "direction" vector
   The rotation can be composed back by 
   rotation = swing * twist

   have singulARITY in case of swing_rotation close to 180 degrees rotation.
   input quaternion can be nonunit length, in that case output can be nonunit
   if input quaternion unit, output is unit both
*/
template<class T> inline void
swing_twist_decomposition(const quaternion<T> &rotation,const vec<3,T> &direction, quaternion<T> &swing, quaternion<T> &twist)
{
    vec<3,T> ra(rotation.x, rotation.y, rotation.z); // rotation axis
    vec<3,T> p(ra.x * direction.x, ra.y * direction.y, ra.z * direction.z);
    twist.set(p.x, p.y, p.z, rotation.w);
    twist.normalize();
    swing = rotation * twist.conjugated();
}

}

template<class T> inline math::quaternion<T>
operator *(const math::quaternion<T> &lhs, const T &rhs) {
	math::quaternion<T> temp = lhs;
	temp *= rhs;
	return temp;
}

template<class T> inline math::quaternion<T>
operator *(const math::quaternion<T> &lhs, const math::quaternion<T> &rhs) {
	math::quaternion<T> temp = lhs;
	temp *= rhs;
	return temp;
}
