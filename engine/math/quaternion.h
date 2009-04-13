#pragma once


#include <math/scalar.h>
#include <math/vec.h>

namespace math {
	template<int M,int N,class T> class matrix;
	template<class T = scalar> class quaternion;

	template<class T> inline T
	inner_product(const quaternion<T> &q0, const quaternion<T> &q1);

	template<class T> inline quaternion<T>
	lerp_unit(const quaternion<T> &q1, const quaternion<T> &q2, T t );

	template<class T> class quaternion_lerper;
	template<class T> class quaternion_slerper;

	template<class T> inline vec<3,T> 
	ortho_project_on_sphere( T x , T y );

	template<class T> inline quaternion<T> 
	arcball(const vec<3,T> &from, const vec<3,T> &to);

	template<class T> inline quaternion<T>
	trackball( const vec<3,T> &from, const vec<3,T> &to );

	template<class T> inline void
	swing_twist_decomposition( const math::quaternion<T> &rotation,const math::vec<3,T> &direction,
		math::quaternion<T> &swing,math::quaternion<T> &twist);
}

template<class T>
class math::quaternion {
public:
	// типы и константы
	typedef T scalar_type;
	typedef std::numeric_limits<scalar_type> limits_type;
	typedef vec<3,T> vec_type;
	typedef matrix<3,3,scalar_type> matrix_type;
	typedef quaternion<T> quaternion_type;

	// данные
    	scalar_type x,y,z,w;

    	quaternion() {
	}

    	quaternion(scalar_type x0, scalar_type y0, scalar_type z0, scalar_type w0):
		x(x0),
		y(y0),
		z(z0),
		w(w0)
	{
	}

    	void set(scalar_type x0, scalar_type y0, scalar_type z0, scalar_type w0) {
        	x = x0;
		y = y0;
		z = z0;
		w = w0;
    	}

    	void set(const quaternion_type& q) {
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

    	void conjugated(quaternion_type &q) const {
        	q = *this;
		q.conjugate();
    	} 

    	quaternion_type conjugated() const {
        	quaternion_type q;
		conjugated(q);
		return q;
    	} 

    	void scale(scalar_type s) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;
    	}

    	scalar_type norm() const {
        	return x*x + y*y + z*z + w*w;
    	}

    	scalar_type magnitude() const {
        	return (scalar_type) sqrt(norm());
    	}

	void invert() {
        	scalar_type in = 1 / norm();
        	set(-x*in, -y*in, -z*in, w*in);
    	}

	void inverted(quaternion_type &q) {
		q=*this;
		q.invert();
	}

    	quaternion_type inverted() const {
		quaternion_type q;
		inverted(q);
		return q;
    	} 

        /**
          stabilize quaternion length within 1 - 1/4
          this operation is a lot faster than normalization
          and preserve length goes to 0 or infinity
        */
        void stabilize_length() {
		scalar_type cs = (scalar_type)(fabs(x) + fabs(y) + fabs(z) + fabs(w));
       		if( cs > 0.0f )
           		set( x/=cs,  y/=cs,  z/=cs,  w/=cs ); 
       		else
           		identity();
    	}

   	/**
 	  scale quaternion that its norm goes to 1 
          the appearing of 0 magnitude or near is a error,
          so we can be sure that can divide by magnitude
    	*/
   	void normalize() {
        	scalar_type m = magnitude();
        	if(m < limits_type::round_error()) {
            		stabilize_length();
            		m = magnitude();
        	}
        	scale( 1/m );
    	}

    	bool equal(const quaternion_type& q){
		return ( fabs(q.x - x) < limits_type::epsilon() ) &&
               ( fabs(q.y - y) < limits_type::epsilon() ) &&
               ( fabs(q.z - z) < limits_type::epsilon() ) &&
               ( fabs(q.w - w) < limits_type::epsilon() );
    	}

    	//----operators------------------------------------------------
	inline quaternion_type operator +() const {
       		return *this;
 	}

    	quaternion_type operator -() const {
        	return quaternion_type(-x, -y, -z, -w);
    	}

    	quaternion_type &operator +=(const quaternion_type& q) {
        	x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;

		return *this;
    	}

    	quaternion_type &operator -=(const quaternion& q ) {
       		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;

		return *this;
    	}

    	quaternion_type &operator *=(scalar_type s) {
       		x *= s;
		y *= s;
		z *= s;
		w *= s;

		return *this;
    	}

    	/**
      	q( cross(v,v') + wv' + w'v, ww' - dot(v,v') )
    	*/
    	quaternion_type &operator *=(const quaternion& q) {
		quaternion_type p=*this;
		x = p.w*q.x + p.x*q.w + p.y*q.z - p.z*q.y;
		y = p.w*q.y + p.y*q.w + p.z*q.x - p.x*q.z;
		z = p.w*q.z + p.z*q.w + p.x*q.y - p.y*q.x;
		w = p.w*q.w - p.x*q.x - p.y*q.y - p.z*q.z;

		return *this;
   	}

    	/**
    	  return rotation angle from -PI to PI 
    	*/
    	scalar_type get_rotation_angle() const {
        	if(w < 0) return 2 * atan2(-sqrt(x*x + y*y + z*z), -w);
        	else return 2 * atan2(sqrt(x*x + y*y + z*z),  w);
    	}

    	/**
    	  create a unit quaternion from axis angle representation
    	*/
    	void rotation(const vec_type &axis, scalar_type angle) {
		if(angle) {
	        	vec_type v(axis);
        		v.normalize();
        		scalar_type half_angle = angle * scalar_type(0.5);
        		scalar_type sin_a = (scalar_type) sin(half_angle);
        		set(v.x*sin_a, v.y*sin_a, v.z*sin_a, cos(half_angle));
		} else {
			identity();
		}
    	}

	// построить кватернион поворотов на углы alpha, beta, gamma
	void rotation(scalar_type alpha, scalar_type beta, scalar_type gamma) {
		vec_type axis(alpha, beta, gamma);
		scalar_type angle = axis.length();
		rotation(axis, angle);
	}

	// построить кватернион поворотов на углы rot.x, rot.y, rot.z
	void rotation(const vec_type &rot) {
		scalar_type angle = rot.length();
		rotation(rot, angle);
	}

	// получить кватернион поворота вектора (1,0,0) в вектор dir
	void direction(const vec_type &dir) {
		matrix_type m;
		m.direction(dir);

		set_unit(m);
	}

	// умножить настоящий кватернион на кватернион поворота вектора (1,0,0) в вектор dir
	void direct(const vec_type &dir) {
		quaternion_type q;
		q.direction(dir);

		*this *= dir;
	}

    	//-----------------------------------
    	/**
	  convert a quaternion to axis angle representation, 
	  preserve the axis direction and angle from -PI to +PI
    	*/
    	void decompose(vec_type &axis, scalar_type &angle)const {
        	scalar_type vl = sqrt( x*x + y*y + z*z );
        	if( vl > limits_type::epsilon() ) {
            		scalar_type ivl = 1 / vl;
            		axis.set( x*ivl, y*ivl, z*ivl );
            		if( w < 0 )
                		angle = 2 * atan2(-vl, -w); //-PI,0 
            		else
                		angle = 2 * atan2( vl,  w); //0,PI 
        	} else {
            		axis.set(0,0,0);
            		angle = 0;
        	}
    	}

    	//-----------------------------------
    	/**
	  create a unit quaternion by rotation matrix
          martix must contain only rotation (not scale or shear)

          For numerical stability we find first the greatest component of quaternion
          and than search others from this one
	*/
    	void set_unit(const matrix_type &m)
    	{
        	scalar_type tr = m.ij[0][0] + m.ij[1][1] + m.ij[2][2];	// trace of martix
        	if (tr > 0) {	// if trace positive than "w" is biggest component
            		set(m.ij[1][2] - m.ij[2][1], m.ij[2][0] - m.ij[0][2], m.ij[0][1] - m.ij[1][0], tr+1);
            		scale(scalar_type(0.5) / scalar_type(sqrt(w)));	// "w" contain the "norm * 4"

        	} else {
			// Some of vector components is bigger
        		if((m.ij[0][0] > m.ij[1][1] ) && ( m.ij[0][0] > m.ij[2][2])) {
            			set( 1 + m.ij[0][0] - m.ij[1][1] - m.ij[2][2], m.ij[1][0] + m.ij[0][1],
                 			m.ij[2][0] + m.ij[0][2], m.ij[1][2] - m.ij[2][1] );
            			scale(scalar_type(0.5) / scalar_type(sqrt(x)));

        		} else {
        			if (m.ij[1][1] > m.ij[2][2]) {
			        	set( m.ij[1][0] + m.ij[0][1], 1 + m.ij[1][1] - m.ij[0][0] - m.ij[2][2],
				        	m.ij[2][1] + m.ij[1][2], m.ij[2][0] - m.ij[0][2] ); 
            			scale(scalar_type(0.5) / scalar_type(sqrt(y)));

        			} else {
			        	set( m.ij[2][0] + m.ij[0][2], m.ij[2][1] + m.ij[1][2],
                 				1 + m.ij[2][2] - m.ij[0][0] - m.ij[1][1],
						m.ij[0][1] - m.ij[1][0] );
            				scale(scalar_type(0.5) / scalar_type(sqrt(z)));
				}
			}
		}
        }

	//----------------------------------------------------------------------------
    	/**
          create a nonunit quaternion from rotation matrix 
          martix must contain only rotation (not scale or shear)
          the result quaternion length is numerical stable 
        */
    	void set(const matrix_type &m) {
        	scalar_type tr = m.ij[0][0] + m.ij[1][1] + m.ij[2][2]; // trace of martix
        	if (tr > 0){  // if trace positive than "w" is biggest component
            		set( m.ij[1][2] - m.ij[2][1], m.ij[2][0] - m.ij[0][2], m.ij[0][1] - m.ij[1][0], tr + 1 );
        	} else {
			// Some of vector components is bigger
        		if((m.ij[0][0] > m.ij[1][1] ) && ( m.ij[0][0] > m.ij[2][2])) {
            			set( 1 + m.ij[0][0] - m.ij[1][1] - m.ij[2][2], m.ij[1][0] + m.ij[0][1],
                 		m.ij[2][0] + m.ij[0][2], m.ij[1][2] - m.ij[2][1] );
			} else {
				if (m.ij[1][1] > m.ij[2][2]) {
            				set( m.ij[1][0] + m.ij[0][1], 1 + m.ij[1][1] - m.ij[0][0] - m.ij[2][2],
                 				m.ij[2][1] + m.ij[1][2], m.ij[2][0] - m.ij[0][2] ); 
        			} else {
			            set( m.ij[2][0] + m.ij[0][2], m.ij[2][1] + m.ij[1][2],
			                 1 + m.ij[2][2] - m.ij[0][0] - m.ij[1][1], m.ij[0][1] - m.ij[1][0] );
				}
			}
		}
        }

    	//-----------------------------------------------------------
    	/**
	  shortest arc quaternion rotate one vector to another by shortest path.
	  create rotation from -> to, for any length vectors.
    	*/
    	void shortest_arc(const vec_type &from, const vec_type &to)
    	{
        	vec_type c( from*to );
        	set( c.x, c.y, c.z, from & to );
        	normalize();    // if "from" or "to" not unit, normalize quat
        	w += 1;		// reducing angle to halfangle
        	if(w <= limits_type::round_error()) // angle close to PI
        	{
            		if( ( from.z*from.z ) > ( from.x*from.x ) )
                		set( 0, from.z, - from.y, w ); //from*vector3(1,0,0) 
            		else 
                		set( from.y, - from.x, 0, w ); //from*vector3(0,0,1) 
        	}

        	normalize(); 
    	}
};

template<class T> T
math::inner_product(const quaternion<T> &q0, const quaternion<T> &q1) {
    return q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.w*q1.w;
}

template<class T> inline math::quaternion<T>
operator +(const math::quaternion<T> &lhs, const math::quaternion<T> &rhs) {
	math::quaternion<T> temp = lhs;
	temp += rhs;
	return temp;
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

/**
  perform linear interpolation (approximate rotation interpolation) of two unit quats,
  result quaternion nonunit, its length lay between. sqrt(2)/2  and 1.0
*/
template<class T> inline math::quaternion<T>
math::lerp_unit(const math::quaternion<T> &q1, const math::quaternion<T> &q2, T t ) {
    T inner = inner_product( q1, q2 );
    if(inner < 0 ){
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
class math::quaternion_lerper
{
public:
	typedef T scalar_type;
	typedef quaternion<scalar_type> quaternion_type;

private:
    quaternion_type  q1, q2;

public:

    void setup_from_unit(const quaternion_type & p, const quaternion_type& q){
        q1 =  p;
        q2 =  q;
        scalar_type inner = inner_product( q1, q2 );
        if(inner < 0 ) q2 = -q2;
        q2 -= q1;
    }

    void setup(const quaternion_type &p, const quaternion_type &q){
        q1 =  p;     q1.normalize();
        q2 =  q;     q2.normalize();
        setup_from_unit( q1, q2);
    }

    void interpolate( scalar_type t, quaternion_type & result_q){
          result_q = q1 + q2*t;
    }
};
/**
 Perform Spherical Linear Interpolation of the quaternions,
 return unit length quaternion 
*/
template<class T>
class math::quaternion_slerper
{
public:
	typedef T scalar_type;
	typedef std::numeric_limits<scalar_type> limits_type;
	typedef quaternion<T> quaternion_type;

private:
    quaternion_type q1, q2;
    scalar_type omega;

public:

    void setup_from_unit(const quaternion_type &p, const quaternion_type &q){
        q1 = p;
        q2 = q;
        scalar_type cos_omega = inner_product(q1, q2);
        if(cos_omega < 0 )
        {
            cos_omega = -cos_omega;
            q2 = -q2;
        }
        if(cos_omega > (1-limits_type::round_error) ) cos_omega = 1-limits_type::round_error;
        omega = (scalar_type)acos( cos_omega );
        scalar_type inv_sin_omega = (scalar_type)(1/sin( omega ));
        q1.scale(inv_sin_omega);
        q2.scale(inv_sin_omega);
    }

    void setup(const quaternion_type &p, const quaternion_type &q){
        q1 = p;
        q1.normalize();
        q2 = q;
        q2.normalize();
        setup_from_unit(q1, q2);
    }

    void interpolate( scalar_type t, quaternion_type& result_q ){
        result_q = q1*(scalar_type)sin( (1 - t)*omega ) + q2*(scalar_type)sin( t*omega );
    }
};

/**
 help function for arcball 
 orthogonal projection on sphere of radius 1, 
 standing in (x = 0, y = 0)
*/
template<class T> math::vec<3,T>
math::ortho_project_on_sphere(T x , T y)
{
    vec<3,T> p(x, y, 0);
    scalar ls = p.length_sq();
    if ( ls >= 1 )
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
template<class T> math::quaternion<T>
math::arcball(const vec<3,T> &from, const vec<3,T> &to)
{
    vec<3,T> p_f = ortho_project_on_sphere(from.x, from.y );
    vec<3,T> p_t = ortho_project_on_sphere(to.x, to.y );
    vec<3,T> c   = cross(p_f, p_t);
    T d   = dot(p_f, p_t);
    return  quaternion<T>(c.x, c.y, c.z, d);
}

/**
 rotate as a half arcball, more realistic manner (like 3dmax).
*/
template<class T> math::quaternion<T>
math::trackball( const vec<3,T> &from, const vec<3,T> &to )
{
    quaternion<T> rot;
    rot.shortest_arc( ortho_project_on_sphere(from.x, from.y ),
                      ortho_project_on_sphere(to.x, to.y ) );
    return rot;
}

/**
   Decompose the rotation on to 2 parts.
   1. Twist - rotation around the "direction" vector
   2. Swing - rotation around axis that is perpendicular to "direction" vector
   The rotation can be composed back by 
   rotation = swing * twist

   have singularity in case of swing_rotation close to 180 degrees rotation.
   input quaternion can be nonunit length, in that case output can be nonunit
   if input quaternion unit, output is unit both
*/
template<class T> inline void
math::swing_twist_decomposition(const math::quaternion<T> &rotation,const math::vec<3,T> &direction,
	math::quaternion<T> &swing,math::quaternion<T> &twist)
{
    vec<3,T> ra(rotation.x, rotation.y, rotation.z); // rotation axis
    vec<3,T> p(ra.x * direction.x, ra.y * direction.y, ra.z * direction.z);
    twist.set(p.x, p.y, p.z, rotation.w );
    twist.normalize();
    swing = rotation * twist.conjugated();
}

namespace math {
	template<class Char,class Traits,class Scalar> std::basic_ostream<Char, Traits> &
	operator <<(std::basic_ostream<Char, Traits> &os, const math::quaternion<Scalar> &q) {
		return os<<q.x<<' '<<q.y<<' '<<q.z<<' '<<q.w;
	}

	template<class Char,class Traits,class Scalar> std::basic_istream<Char, Traits> &
	operator >>(std::basic_istream<Char, Traits> &is, math::quaternion<Scalar> &q) {
		return is>>q.x>>q.y>>q.z>>q.w;
	}
}

