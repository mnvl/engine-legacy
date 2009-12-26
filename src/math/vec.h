#pragma once

#include <iosfwd>
#include <algorithm>
#include <lua.h>
#include <math/scalar.h>

namespace math
{

template<int N,class T=scalar> class vec;
template<class T> class quaternion;
template<class T> class vec<2,T>;
template<class T> class vec<3,T>;
template<class T> class vec<4,T>;

template<class T> inline void add(vec<2,T> &c,const vec<2,T> &a,const vec<2,T> &b);
template<class T> inline void add(vec<3,T> &c,const vec<3,T> &a,const vec<3,T> &b);
template<class T> inline void add(vec<4,T> &c,const vec<4,T> &a,const vec<4,T> &b);
template<int N,class T> inline void add(vec<N,T> &c,const vec<N,T> &a,const vec<N,T> &b);

template<class T> inline void sub(vec<2,T> &c,const vec<2,T> &a,const vec<2,T> &b);
template<class T> inline void sub(vec<3,T> &c,const vec<3,T> &a,const vec<3,T> &b);
template<class T> inline void sub(vec<4,T> &c,const vec<4,T> &a,const vec<4,T> &b);
template<int N,class T> inline void sub(vec<N,T> &c,const vec<N,T> &a,const vec<N,T> &b);

template<class T> inline void mul(vec<2,T> &c,const vec<2,T> &a,const vec<2,T> &b);
template<class T> inline void mul(vec<3,T> &c,const vec<3,T> &a,const vec<3,T> &b);
template<class T> inline void mul(vec<4,T> &c,const vec<4,T> &a,const vec<4,T> &b);
template<int N,class T> inline void mul(vec<N,T> &c,const vec<N,T> &a,const vec<N,T> &b);

template<class T> inline void mul(vec<2,T> &c,const vec<2,T> &a,T b);
template<class T> inline void mul(vec<3,T> &c,const vec<3,T> &a,T b);
template<class T> inline void mul(vec<4,T> &c,const vec<4,T> &a,T b);
template<int N,class T> inline void mul(vec<N,T> &c,const vec<N,T> &a,T b);

template<class T> inline void div(vec<2,T> &c,const vec<2,T> &a,const vec<2,T> &b);
template<class T> inline void div(vec<3,T> &c,const vec<3,T> &a,const vec<3,T> &b);
template<class T> inline void div(vec<4,T> &c,const vec<4,T> &a,const vec<4,T> &b);
template<int N,class T> inline void div(vec<N,T> &c,const vec<N,T> &a,const vec<N,T> &b);

template<class T> inline void div(vec<2,T> &c,const vec<2,T> &a,T b);
template<class T> inline void div(vec<3,T> &c,const vec<3,T> &a,T b);
template<class T> inline void div(vec<4,T> &c,const vec<4,T> &a,T b);
template<int N,class T> inline void div(vec<N,T> &c,const vec<N,T> &a,T b);

template<class T> inline T dot_product(const vec<2,T> &a,const vec<2,T> &b);
template<class T> inline T dot_product(const vec<3,T> &a,const vec<3,T> &b);
template<class T> inline T dot_product(const vec<4,T> &a,const vec<4,T> &b);
template<int N,class T> inline T dot_product(const vec<N,T> &a,const vec<N,T> &b);

template<class T> inline void cross_product(vec<3,T> &u,const vec<3,T> &v1,const vec<3,T> &v2);

template<class T> inline void minmax(vec<2,T> &v1,vec<2,T> &v2);
template<class T> inline void minmax(vec<3,T> &v1,vec<3,T> &v2);
template<class T> inline void minmax(vec<4,T> &v1,vec<4,T> &v2);

template<int N,class T> inline vec<N,T> normalize(const vec<N,T> &v);

void bind_vec(lua_State *L);

#pragma pack(push, 1)
template<class T>
class vec<2,T> {
public:
	typedef T scalar_type;
	typedef size_t size_type;
	static const size_type arity = 2;
	typedef vec<arity,scalar_type> vec_type;
	typedef size_type mask_type;

	union {
		struct {
			scalar_type x,y;
		};

		struct {
			scalar_type u,v;
		};

		struct {
			scalar_type s,t;
		};

		struct {
			scalar_type A,B;
		};

		scalar_type i[arity];
	};

	vec() {
	}

	vec(const mask_type mask) {
		foundation(mask);
	}

	vec(const vec_type &v):
	x(v.x), y(v.y) {
	}

	template<class type>
	vec(const vec<arity,type> &v):
	x((scalar_type) v.x), y((scalar_type) v.y) {
	}


	vec(scalar_type x0,scalar_type y0):
	x(x0), y(y0) {
	}

	void set(scalar_type x0, scalar_type y0) {
		x = x0;
		y = y0;
	}

	void set_all(scalar_type s) {
		set(s, s);
	}

	vec_type &operator =(const vec_type &v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	vec_type operator -() const {
		return vec_type(-x, -y);
	}

	vec_type &operator *=(const scalar_type k) {
		mul(*this, *this, k);
		return *this;
	}

	vec_type &operator /=(const scalar_type k) {
		div(*this, *this, k);
		return *this;
	}

	vec_type &operator +=(const vec_type &v) {
		add(*this, *this, v);
		return *this;
	}

	vec_type &operator -=(const vec_type &v) {
		sub(*this, *this, v);
		return *this;
	}

	vec_type &operator *=(const vec_type &v) {
		mul(*this, *this, v);
		return *this;
	}

	vec_type &operator /=(const vec_type &v) {
		div(*this, *this, v);
		return *this;
	}

	bool operator <(vec_type const &rhs) const {
		return x < rhs.x && y < rhs.y;
	}

	bool operator >(vec_type const &rhs) const {
		return x > rhs.x && y > rhs.y;
	}

	scalar_type length_sq() const {
		return dot_product(*this, *this);
	}

	scalar_type length() const {
		return sqrt(length_sq());
	}

	scalar_type length_manhattan() const {
		return abs(x) + abs(y);
	}

	void normalize() {
		scalar_type l = length();
		if(abs(l) > 0) *this /= l;
	}

	void negate() {
		operator *=(scalar_type(-1));
	}

	mask_type segment() const {
		mask_type mask=0;
		mask += (x > 0) << 0;
		mask += (y > 0) << 1;
		return mask;
	}

	void foundation(mask_type mask) {
		x = scalar_type(mask & (1 << 0));
		y = scalar_type(mask & (1 << 1) ? 1 : 0);
	}

	void projection(mask_type mask) {
		x *= mask & (1 << 0);
		y *= mask & (1 << 1);
	}

	void select(mask_type mask,const vec_type &v1,const vec_type v2) {
		x = mask & (1<<0) ? v1.x : v2.x;
		y = mask & (1<<1) ? v1.y : v2.y;
	}

	bool collinear(const vec_type &v) const {
		return abs(x*v.y - y*v.x) < EPSILON;
	}

	void to_polar(scalar_type &modulus, scalar_type &angle) const;
};
#pragma pack(pop)

#pragma pack(push, 1)
template<class T>
class vec<3,T> {
public:
	typedef T scalar_type;
	typedef size_t size_type;
	static const size_type arity = 3;
	typedef vec<2, scalar_type> vec2_type;
	typedef vec<arity,scalar_type> vec_type;
	typedef size_type mask_type;

	union {
		struct {
			scalar_type x,y,z;
		};

		struct {
			scalar_type r,g,b;
		};

		struct {
			scalar_type A,B,C;
		};

		scalar_type i[arity];
	};

	vec() {
	}

	vec(const mask_type mask) {
		foundation(mask);
	}

	vec(vec2_type const &v):
	x(v.x), y(v.y), z(0)
	{
	}

	vec(vec_type const &v):
	x(v.x), y(v.y), z(v.z)
	{
	}

	vec(scalar_type x0,scalar_type y0,scalar_type z0):
	x(x0), y(y0), z(z0)
	{
	}

	void set(const scalar_type x0,const scalar_type y0,const scalar_type z0) {
		x = x0;
		y = y0;
		z = z0;
	}

	#if defined(D3DVECTOR_DEFINED)
	vec(const D3DVECTOR &v):
	x(scalar_type(v.x)), y(scalar_type(v.y)), z(scalar_type(v.z)) {
	}
	#endif

	void neg() {
		x = -x;
		y = -y;
		z = -z;
	}

	vec_type operator -() const {
		return vec_type(-x, -y, -z);
	}

	vec_type &operator =(const vec_type &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	#if defined(D3DVECTOR_DEFINED)
	vec_type &operator =(const D3DVECTOR &v) {
		x = scalar_type(v.x);
		y = scalar_type(v.y);
		z = scalar_type(v.z);
		return *this;
	}
	#endif

	#if defined(D3DVECTOR_DEFINED)
	operator D3DVECTOR() const {
		D3DVECTOR v={float(x),float(y),float(z)};
		return v;
	}
	#endif

	vec_type &operator *=(const scalar_type k) {
		mul(*this, *this, k);
		return *this;
	}

	vec_type &operator /=(const scalar_type k) {
		div(*this, *this, k);
		return *this;
	}

	vec_type &operator +=(const vec_type &v) {
		add(*this, *this, v);
		return *this;
	}

	vec_type &operator -=(const vec_type &v) {
		sub(*this, *this, v);
		return *this;
	}

	vec_type &operator *=(const vec_type &v) {
		mul(*this, *this, v);
		return *this;
	}

	vec_type &operator /=(const vec_type &v) {
		div(*this, *this, v);
		return *this;
	}

	vec_type operator +(vec_type const &rhs) const {
		vec_type res;
		add(res, *this, rhs);
		return res;
	}

	vec_type operator -(vec_type const &rhs) const {
		vec_type res;
		sub(res, *this, rhs);
		return res;
	}

	vec_type operator *(scalar_type const s) const {
		return vec_type(x * s, y * s, z * s);
	}

	bool operator <(vec_type const &rhs) const {
		return x < rhs.x && y < rhs.y && z < rhs.z;
	}

	bool operator >(vec_type const &rhs) const {
		return x > rhs.x && y > rhs.y && z > rhs.z;
	}

	scalar_type length_sq() const {
		return dot_product(*this, *this);
	}

	scalar_type length() const {
		return sqrt(length_sq());
	}

	void normalize() {
		scalar_type l = length();
		if(abs(l) > 0) *this /= l;
	}

	void negate() {
		operator *=(scalar_type(-1));
	}

	mask_type segment() const {
		mask_type mask=0;
		mask += (x > 0) << 0;
		mask += (y > 0) << 1;
		mask += (z > 0) << 2;
		return mask;
	}

	void foundation(const mask_type mask) {
		x = scalar_type(mask & (1 << 0) ? 1 : 0);
		y = scalar_type(mask & (1 << 1) ? 1 : 0);
		z = scalar_type(mask & (1 << 2) ? 1 : 0);
	}

	void projection(const mask_type mask) {
		x *= mask & (1 << 0) ? 1 : 0;
		y *= mask & (1 << 1) ? 1 : 0;
		z *= mask & (1 << 2) ? 1 : 0;
	}

	void select(mask_type mask,const vec_type &v1,const vec_type v2) {
		x = mask & (1 << 0) ? v1.x : v2.x;
		y = mask & (1 << 1) ? v1.y : v2.y;
		z = mask & (1 << 2) ? v1.z : v2.z;
	}

	void to_axis_angle(vec_type &axis, scalar_type &angle) const;
};
#pragma pack(pop)

#pragma pack(push, 1)
template<class T>
class vec<4,T> {
public:
	typedef T scalar_type;
	typedef size_t size_type;
	static const size_type arity = 4;
	typedef vec<arity,scalar_type> vec_type;
	typedef size_type mask_type;

	union {
		struct {
			scalar_type x,y,z,w;
		};

		struct {
			scalar_type r,g,b,a;
		};

		struct {
			scalar_type A,B,C,D;
		};

		scalar_type i[arity];
	};

	vec() {
	}

	vec(const mask_type mask) {
		foundation(mask);
	}

	vec(const vec_type &v):
	x(v.x), y(v.y), z(v.z), w(v.w) {
	}

	vec(vec<arity - 1, scalar_type> const &v):
	x(v.x), y(v.y), z(v.z), w(1) {
	}

	vec(scalar_type x0,scalar_type y0,scalar_type z0,scalar_type w0):
	x(x0), y(y0), z(z0), w(w0) {
	}

	#if defined(D3DCOLORVALUE_DEFINED)
	vec(const D3DCOLORVALUE &v):
	r(scalar_type(v.r)),g(scalar_type(v.g)),b(scalar_type(v.b)),a(scalar_type(v.a)) {
	}
	#endif

	void set(scalar_type x0,scalar_type y0,scalar_type z0,scalar_type w0) {
		x = x0;
		y = y0;
		z = z0;
		w = w0;
	}

	vec_type &operator =(const vec_type &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	}

	#if defined(D3DCOLORVALUE_DEFINED)
	vec_type &operator =(const D3DCOLORVALUE &v) {
		r = scalar_type(v.r);
		g = scalar_type(v.g);
		b = scalar_type(v.b);
		a = scalar_type(v.a);
		return *this;
	}
	#endif

	#if defined(D3DCOLORVALUE_DEFINED)
	operator D3DCOLORVALUE() const {
		D3DCOLORVALUE c={float(r),float(g),float(b),float(a)};
		return c;
	}
	#endif

	vec_type &operator *=(const scalar_type k) {
		mul(*this, *this, k);
		return *this;
	}

	vec_type &operator /=(const scalar_type k) {
		div(*this, *this, k);
		return *this;
	}

	vec_type &operator +=(const vec_type &v) {
		add(*this, *this, v);
		return *this;
	}

	vec_type &operator -=(const vec_type &v) {
		sub(*this, *this, v);
		return *this;
	}

	vec_type &operator *=(const vec_type &v) {
		mul(*this, *this, v);
		return *this;
	}

	vec_type &operator /=(const vec_type &v) {
		div(*this, *this, v);
		return *this;
	}

	bool operator <(vec_type const &rhs) const {
		return x < rhs.x && y < rhs.y && z < rhs.z && w < rhs.w;
	}

	bool operator >(vec_type const &rhs) const {
		return x > rhs.x && y > rhs.y && z > rhs.z && w > rhs.w;
	}

	scalar_type length_sq() const {
		return dot_product(*this, *this);
	}

	scalar_type length() const {
		return sqrt(length_sq());
	}

	void normalize() {
		scalar_type l=length();
		if(abs(l) > 0) *this /= l;
	}

	void negate() {
		operator *=(scalar_type(-1));
	}

	mask_type segment() const {
		mask_type mask=0;
		mask += (x > 0) << 0;
		mask += (y > 0) << 1;
		mask += (z > 0) << 2;
		mask += (w > 0) << 3;
		return mask;
	}

	void foundation(const mask_type mask) {
		x = scalar_type(mask & (1 << 0) ? 1 : 0);
		y = scalar_type(mask & (1 << 1) ? 1 : 0);
		z = scalar_type(mask & (1 << 2) ? 1 : 0);
		w = scalar_type(mask & (1 << 3) ? 1 : 0);
	}

	void projection(const mask_type mask) {
		x *= mask & (1<<0) ? 1 : 0;
		y *= mask & (1<<1) ? 1 : 0;
		z *= mask & (1<<2) ? 1 : 0;
		w *= mask & (1<<3) ? 1 : 0;
	}

	void select(mask_type mask,const vec_type &v1,const vec_type v2) {
		x = mask & (1<<0) ? v1.x : v2.x;
		y = mask & (1<<1) ? v1.y : v2.y;
		z = mask & (1<<2) ? v1.z : v2.z;
		w = mask & (1<<3) ? v1.w : v2.w;
	}
};
#pragma pack(pop)

#pragma pack(push, 1)
template<int N,class T>
class vec {
public:
    typedef size_t size_type;
	static const size_type arity=N;
	typedef T scalar_type;
	typedef vec<arity,scalar_type> vec_type;
	typedef int mask_type;

	scalar_type i[arity];

    void null() {
        std::fill(i, i+arity, scalar_type(0));
    }

	vec_type &operator *=(const scalar_type k) {
		mul(*this, *this, k);
		return *this;
	}

	vec_type &operator /=(const scalar_type k) {
		div(*this, *this, k);
		return *this;
	}

    vec_type &operator =(const vec_type &v) {
        std::copy(v.i, v.i + arity, i);
        return *this;
    }

    template<class X>
    vec_type &operator =(const vec<arity, X> &v) {
        for (size_t index = 0; index < arity; index++)
            i[index] = (scalar_type) v.i[index];
        return *this;
    }

	vec_type &operator +=(const vec_type &v) {
		add(*this, *this, v);
		return *this;
	}

	vec_type &operator -=(const vec_type &v) {
		sub(*this, *this, v);
		return *this;
	}

	vec_type &operator *=(const vec_type &v) {
		mul(*this, *this, v);
		return *this;
	}

	vec_type &operator /=(const vec_type &v) {
		div(*this, *this, v);
		return *this;
	}

	bool operator <(vec_type const &rhs) const {
		for (size_t n = 0; n < arity; n++)
			if (i[n] >= rhs.i[n]) return false;
		return true;
	}

	bool operator >(vec_type const &rhs) const {
		for (size_t n = 0; n > arity; n++)
			if (i[n] <= rhs.i[n]) return false;
		return true;
	}

	scalar_type length_sq() const {
		return dot_product(*this, *this);
	}

	scalar_type length() const {
		return sqrt(length_sq());
	}

	void normalize() {
		scalar_type l=length();
		if(abs(l) > 0) *this /= l;
	}
};
#pragma pack(pop)

template<class L,class R> inline bool lexicagraphical_less(const vec<2,L> &left,const vec<2,R> &right) {
	if (left.x < right.x) return true;
	if (left.x > right.x) return false;

	return left.y < right.y;
}

template<class L,class R> inline bool lexicagraphical_less(const vec<3,L> &left,const vec<3,R> &right) {
	if (left.x < right.x) return true;
	if (left.x > right.x) return false;

	if (left.y < right.y) return true;
	if (left.y > right.y) return false;

	return left.z < right.z;
}

template<class L,class R> inline bool lexicagraphical_less(const vec<4,L> &left,const vec<4,R> &right) {
	if (left.x < right.x) return true;
	if (left.x > right.x) return false;

	if (left.y < right.y) return true;
	if (left.y > right.y) return false;

	if (left.z < right.z) return true;
	if (left.z > right.z) return false;

	return left.w < right.w;
}

template<int N,class L,class R> inline bool lexicagraphical_greater(const vec<N,L> &left,const vec<N,R> &right) {
	return lexicagraphical_less(right, left);
}

template<class T> void
add(vec<2,T> &c,const vec<2,T> &a,const vec<2,T> &b) {
	c.i[0] = a.i[0] + b.i[0];
	c.i[1] = a.i[1] + b.i[1];
}

template<class T> void
add(vec<3,T> &c,const vec<3,T> &a,const vec<3,T> &b) {
	c.i[0] = a.i[0] + b.i[0];
	c.i[1] = a.i[1] + b.i[1];
	c.i[2] = a.i[2] + b.i[2];
}

template<class T> void
add(vec<4,T> &c,const vec<4,T> &a,const vec<4,T> &b) {
	c.i[0] = a.i[0] + b.i[0];
	c.i[1] = a.i[1] + b.i[1];
	c.i[2] = a.i[2] + b.i[2];
	c.i[3] = a.i[3] + b.i[3];
}

template<int N,class T> void
add(vec<N,T> &c,const vec<N,T> &a,const vec<N,T> &b) {
	for(int i=0; i<N; i++)
		c.i[i] = a.i[i] + b.i[i];
}

template<class T> void
sub(vec<2,T> &c,const vec<2,T> &a,const vec<2,T> &b) {
	c.i[0] = a.i[0] - b.i[0];
	c.i[1] = a.i[1] - b.i[1];
}

template<class T> void
sub(vec<3,T> &c,const vec<3,T> &a,const vec<3,T> &b) {
	c.i[0] = a.i[0] - b.i[0];
	c.i[1] = a.i[1] - b.i[1];
	c.i[2] = a.i[2] - b.i[2];
}
template<class T> void
sub(vec<4,T> &c,const vec<4,T> &a,const vec<4,T> &b) {
	c.i[0] = a.i[0] - b.i[0];
	c.i[1] = a.i[1] - b.i[1];
	c.i[2] = a.i[2] - b.i[2];
	c.i[3] = a.i[3] - b.i[3];
}

template<int N,class T> void
sub(vec<N,T> &c,const vec<N,T> &a,const vec<N,T> &b) {
	for(int i=0; i<N; i++)
		c.i[i] = a.i[i] - b.i[i];
}

template<class T> void
mul(vec<2,T> &c,const vec<2,T> &a,const vec<2,T> &b) {
	c.i[0] = a.i[0] * b.i[0];
	c.i[1] = a.i[1] * b.i[1];
}

template<class T> void
mul(vec<3,T> &c,const vec<3,T> &a,const vec<3,T> &b) {
	c.i[0] = a.i[0] * b.i[0];
	c.i[1] = a.i[1] * b.i[1];
	c.i[2] = a.i[2] * b.i[2];
}

template<class T> void
mul(vec<4,T> &c,const vec<4,T> &a,const vec<4,T> &b) {
	c.i[0] = a.i[0] * b.i[0];
	c.i[1] = a.i[1] * b.i[1];
	c.i[2] = a.i[2] * b.i[2];
	c.i[3] = a.i[3] * b.i[3];
}

template<int N,class T> void
mul(vec<N,T> &c,const vec<N,T> &a,const vec<N,T> &b) {
	for(int i=0; i<N; i++)
		c.i[0] = a.i[0] * b.i[0];
}

template<class T> void
mul(vec<2,T> &c,const vec<2,T> &a,T b) {
	c.i[0] = a.i[0] * b;
	c.i[1] = a.i[1] * b;
}

template<class T> void
mul(vec<3,T> &c,const vec<3,T> &a,T b) {
	c.i[0] = a.i[0] * b;
	c.i[1] = a.i[1] * b;
	c.i[2] = a.i[2] * b;
}

template<class T> void
mul(vec<4,T> &c,const vec<4,T> &a,T b) {
	c.i[0] = a.i[0] * b;
	c.i[1] = a.i[1] * b;
	c.i[2] = a.i[2] * b;
	c.i[3] = a.i[3] * b;
}

template<int N,class T> void
mul(vec<N,T> &c,const vec<N,T> &a,T b) {
	for(int i=0; i<N; i++)
		c.i[i] = a.i[i] * b;
}

template<class T> void
div(vec<2,T> &c,const vec<2,T> &a,const vec<2,T> &b) {
	c.i[0] = a.i[0] / b.i[0];
	c.i[1] = a.i[1] / b.i[1];
}

template<class T> void
div(vec<3,T> &c,const vec<3,T> &a,const vec<3,T> &b) {
	c.i[0] = a.i[0] / b.i[0];
	c.i[1] = a.i[1] / b.i[1];
	c.i[2] = a.i[2] / b.i[2];
}

template<class T> void
div(vec<4,T> &c,const vec<4,T> &a,const vec<4,T> &b) {
	c.i[0] = a.i[0] / b.i[0];
	c.i[1] = a.i[1] / b.i[1];
	c.i[2] = a.i[2] / b.i[2];
	c.i[3] = a.i[3] / b.i[3];
}

template<int N,class T> void
div(vec<N,T> &c,const vec<N,T> &a,const vec<N,T> &b) {
	for(int i=0; i<N; i++)
		c.i[0] = a.i[0] / b.i[0];
}

template<class T> void
div(vec<2,T> &c,const vec<2,T> &a,T b) {
	c.i[0] = a.i[0] / b;
	c.i[1] = a.i[1] / b;
}

template<class T> void
div(vec<3,T> &c,const vec<3,T> &a,T b) {
	c.i[0] = a.i[0] / b;
	c.i[1] = a.i[1] / b;
	c.i[2] = a.i[2] / b;
}

template<class T> void
div(vec<4,T> &c,const vec<4,T> &a,T b) {
	c.i[0] = a.i[0] / b;
	c.i[1] = a.i[1] / b;
	c.i[2] = a.i[2] / b;
	c.i[3] = a.i[3] / b;
}

template<int N,class T> void
div(vec<N,T> &c,const vec<N,T> &a,T b) {
	for(int i=0; i<N; i++)
		c.i[i] = a.i[i] / b;
}

template<class T> T
dot_product(const vec<2,T> &a,const vec<2,T> &b) {
	return a.i[0]*b.i[0] + a.i[1]*b.i[1];
}

template<class T> T
dot_product(const vec<3,T> &a,const vec<3,T> &b) {
	return a.i[0]*b.i[0] + a.i[1]*b.i[1] + a.i[2]*b.i[2];
}

template<class T> T
dot_product(const vec<4,T> &a,const vec<4,T> &b) {
	return a.i[0]*b.i[0] + a.i[1]*b.i[1] + a.i[2]*b.i[2] + a.i[3]*b.i[3];
}
template<int N,class T> T
dot_product(const vec<N,T> &a,const vec<N,T> &b) {
	T sum=a.i[0]*b.i[0];
	for(int i=1; i<N; i++)
		sum += a.i[i]*b.i[i];
}

template<class T> void
cross_product(vec<3,T> &u,const vec<3,T> &v1,const vec<3,T> &v2) {
	u.x = v1.y * v2.z - v1.z * v2.y;
	u.y = v1.z * v2.x - v1.x * v2.z;
	u.z = v1.x * v2.y - v1.y * v2.x;
}

template<class T> inline void
minmax(vec<2,T> &v1,vec<2,T> &v2) {
	if(v1.x>v2.x) std::swap(v1.x,v2.x);
	if(v1.y>v2.y) std::swap(v1.y,v2.y);
}

template<class T> inline void
minmax(vec<3,T> &v1,vec<3,T> &v2) {
	if(v1.x>v2.x) std::swap(v1.x,v2.x);
	if(v1.y>v2.y) std::swap(v1.y,v2.y);
	if(v1.z>v2.z) std::swap(v1.z,v2.z);
}

template<class T> inline void
minmax(vec<4,T> &v1,vec<4,T> &v2) {
	if(v1.x>v2.x) std::swap(v1.x,v2.x);
	if(v1.y>v2.y) std::swap(v1.y,v2.y);
	if(v1.z>v2.z) std::swap(v1.z,v2.z);
	if(v1.w>v2.w) std::swap(v1.w,v2.w);
}

template<int N,class T> vec<N,T>
normalize(const vec<N,T> &v) {
	vec<N,T> temp(v);
	temp.normalize();
	return temp;
}

}

template<int N,class T> inline math::vec<N,T>
operator *(const math::vec<N,T> &lhs,T rhs) {
	math::vec<N,T> res;
	math::mul(res, lhs, rhs);
	return res;
}

template<int N,class T> inline math::vec<N,T>
operator *(T lhs, const math::vec<N,T> &rhs) {
	math::vec<N,T> res;
	math::mul(res, rhs, lhs);
	return res;
}

template<int N,class T> inline math::vec<N,T>
operator /(const math::vec<N,T> &lhs,T rhs) {
	math::vec<N,T> res;
	math::div(res, lhs, rhs);
	return res;
}

template<int N,class T> inline math::vec<N,T>
operator +(const math::vec<N,T> &lhs, const math::vec<N,T> &rhs) {
	math::vec<N,T> res;
	math::add(res, lhs, rhs);
	return res;
}

template<int N,class T> inline math::vec<N,T>
operator -(const math::vec<N,T> &lhs,const math::vec<N,T> &rhs) {
	math::vec<N,T> res;
	math::sub(res, lhs, rhs);
	return res;
}

template<int N,class T> inline math::vec<N,T>
operator *(const math::vec<N,T> &lhs, const math::vec<N,T> &rhs) {
	math::vec<N,T> res;
	math::mul(res, lhs, rhs);
	return res;
}

template<int N,class T> inline math::vec<N,T>
operator /(const math::vec<N,T> &lhs,const math::vec<N,T> &rhs) {
	math::vec<N,T> res;
	math::div(res, lhs, rhs);
	return res;
}

template<int N,class T> inline T
operator &(const math::vec<N,T> &lhs,const math::vec<N,T> &rhs) {
	return math::dot_product(lhs, rhs);
}

template<class T> inline math::vec<3,T>
operator ^(const math::vec<3,T> &lhs,const math::vec<3,T> &rhs) {
	math::vec<3,T> res;
	math::cross_product(res, lhs, rhs);
	return res;
}

template<class T>
std::istream &operator >>(std::istream &is, math::vec<3, T> &v) {
	return is >> v.x >> v.y >> v.z;
}
