#pragma once

#include <engine/math/scalar.h>
#include <engine/math/vec.h>
#include <engine/math/quaternion.h>

namespace math {
	template<int N,int M,class T=scalar> class matrix;
	template<class T> class matrix<3,3,T>;
	template<class T> class matrix<4,4,T>;

	template<class T> inline void mul(vec<2,T> &r,const vec<2,T> &v,const matrix<3,3,T> &m);
	template<class T> inline void mul(vec<3,T> &r,const vec<2,T> &v,const matrix<3,3,T> &m);
	template<class T> inline void mul(vec<3,T> &r,const vec<3,T> &v,const matrix<3,3,T> &m);
	template<class T> inline void mul(vec<3,T> &r,const vec<3,T> &v,const matrix<4,4,T> &m);
	template<class T> inline void mul(vec<4,T> &r,const vec<3,T> &v,const matrix<4,4,T> &m);
	template<class T> inline void mul(vec<4,T> &r,const vec<4,T> &v,const matrix<4,4,T> &m);
	
	template<int M,int N,int K,class T> inline void
	mul(matrix<M,K,T> &result,const matrix<M,N,T> &left,const matrix<N,K,T> &right);
}

template<class T>
class math::matrix<3,3,T> {
public:
	// типы и константы
	typedef size_t size_type;
	static const size_type width=3,height=3;
	typedef T scalar_type;
	typedef vec<width-1,scalar_type> point_type;
	typedef vec<width,scalar_type> vec_type;
	typedef quaternion<scalar_type> quaternion_type;
	typedef matrix<width,height,scalar_type> matrix_type;

	// данные
	scalar_type ij[width][height];

	// конструкторы
	void identity();
	void translation(scalar_type x,scalar_type y);
	void translation(const point_type &p);
	void translate(scalar_type x,scalar_type y);
	void translate(const point_type &p);
	void scaling(scalar_type x, scalar_type y);
	void scaling(const point_type &p);
	void scale(scalar_type x, scalar_type y);
	void scale(const point_type &p);
	void rotation(scalar_type alpha,scalar_type beta,scalar_type gamma);
	void rotation(scalar_type gamma) { rotation(0, 0, gamma); }
	void rotation(const vec_type &v);
	void rotation(const quaternion_type &q);
	void rotation(vec_type const &axis, scalar_type sine, scalar_type cosine);
	void rotation(vec_type const &axis, scalar_type angle);
	void rotate(scalar_type alpha,scalar_type beta,scalar_type gamma);
	void rotate(scalar_type gamma) { rotation(0, 0, gamma); }
	void rotate(const vec_type &v);
	void rotate(const quaternion_type &q);
	void rotate(vec_type const &axis, scalar_type sine, scalar_type cosine);
	void rotate(vec_type const &axis, scalar_type angle);
	void direction(const vec_type &dir);
	void direct(const vec_type &dir);

	// вычислить детерминант минора
	// r0, r1 -- индексы столбцов минора
	// c0, c1 -- индексы строк минора
	scalar_type minor(int r0,int r1,int c0,int c1) const {
		return ij[r0][c0] * ij[r1][c1] - ij[r1][c0] * ij[r0][c1];
	}

	void adjoint(matrix_type &M) const {
		M.ij[0][0] =  minor( 1, 2, 1, 2);
		M.ij[0][1] = -minor( 0, 2, 1, 2);
		M.ij[0][2] =  minor( 0, 1, 1, 2);
		M.ij[1][0] = -minor( 1, 2, 0, 2);
		M.ij[1][1] =  minor( 0, 2, 0, 2);
		M.ij[1][2] = -minor( 0, 1, 0, 2);
		M.ij[2][0] =  minor( 1, 2, 0, 1);
		M.ij[2][1] = -minor( 0, 2, 0, 1);
		M.ij[2][2] =  minor( 0, 1, 0, 1);
	}

	scalar_type determinant() const {
		return 	ij[0][0] * minor(1, 2, 1, 2) -
			ij[0][1] * minor(1, 2, 0, 2) +
			ij[0][2] * minor(1, 2, 0, 1);
	}

	scalar_type trace() const {
		return ij[0][0] + ij[1][1] + ij[2][2];
	}

	// вычислить обратную матрицу и записать ее в M
	// возвращает false если детерминант равен нулю
	bool inverse(matrix_type &M) const {
		scalar_type det = determinant();
		if(det == 0) return false;
		adjoint(M);
		M /= det;
		return true;
	}

	// инвертирует матрицу
	// возвращает false если детерминант равен нулю
	bool invert() {
		return inverse(*this);
	}

	// записать в T транспонированную матрицу
	void transpose(matrix_type &tp) {
        tp.ij[0][0] = ij[0][0];		tp.ij[0][1] = ij[1][0];		tp.ij[0][2] = ij[2][0];
		tp.ij[1][0] = ij[0][1];		tp.ij[1][1] = ij[1][1];		tp.ij[1][2] = ij[2][1];
		tp.ij[2][0] = ij[0][2];		tp.ij[2][1] = ij[1][2];		tp.ij[2][2] = ij[2][2];
	}

	matrix_type &operator *=(scalar_type k) {
		ij[0][0]*=k;	ij[0][1]*=k;	ij[0][2]*=k;
		ij[1][0]*=k;	ij[1][1]*=k;	ij[1][2]*=k;
		ij[2][0]*=k;	ij[2][1]*=k;	ij[2][2]*=k;
		return *this;
	}

	matrix_type &operator *=(const matrix_type &m) {
		matrix_type n=*this;
		mul(*this,n,m);
		return *this;
	}

	matrix_type &operator /=(scalar_type k) {
		return *this *= 1/k;
	}

	void decompose(vec_type &rotation) const;
};

template<class T>
class math::matrix<4,4,T> {
public:
	// типы и константы
	typedef size_t size_type;
	static size_type const width=4,height=4;
	typedef T scalar_type;
	typedef vec<width,scalar_type> vec_type;
	typedef vec<width-1,scalar_type> point_type;
	typedef quaternion<scalar_type> quaternion_type;
	typedef matrix<width,height,scalar_type> matrix_type;

	// данные
	scalar_type ij[width][height];

	// конструкторы
	matrix() {
	}

	#if defined(D3DMATRIX_DEFINED)
	matrix(const D3DMATRIX &right) {
		for(size_type i=0; i<width; i++) {
			for(size_type j=0; j<height; j++) {
				ij[i][j] = scalar_type(right.m[i][j]);
			}
		}
	}
	#endif

	void identity();
	void translation(scalar_type x,scalar_type y,scalar_type z);
	void translation(const point_type &p);
	void translate(scalar_type x,scalar_type y,scalar_type z);
	void translate(const point_type &p);
	void scaling(scalar_type sx,scalar_type sy,scalar_type sz);
	void scaling(const point_type &s);
	void scale(scalar_type sx,scalar_type sy,scalar_type sz);
	void scale(const point_type &s);
	void rotation(scalar_type alpha,scalar_type beta,scalar_type gamma);
	void rotation(const point_type &v);
	void rotation(const quaternion_type &q);
	void rotation(point_type const &axis, scalar_type sine, scalar_type cosine);
	void rotation(point_type const &axis, scalar_type angle);
	void rotate(scalar_type alpha,scalar_type beta,scalar_type gamma);
	void rotate(const point_type &v);
	void rotate(const quaternion_type &q);
	void rotate(point_type const &axis, scalar_type sine, scalar_type cosine);
  	void rotate(point_type const &axis, scalar_type angle);
	void direction(const point_type &dir);
	void direct(const point_type &dir);
	void lookat(const point_type &eye,const point_type &at,const point_type &up);
	void ortho(scalar_type width,scalar_type height,scalar_type z_near,scalar_type z_far);
	void perspective(scalar_type fovy,scalar_type aspect,scalar_type z_near,scalar_type z_far);

	// вычислить детерминант минора
	// r0, r1, r2 -- индексы столбцов минора
	// c0, c1, c2 -- индексы строк минора
	scalar_type minor(int r0,int r1,int r2,int c0,int c1,int c2) const {
		return  ij[r0][c0] * (ij[r1][c1] * ij[r2][c2] - ij[r2][c1] * ij[r1][c2]) -
			ij[r0][c1] * (ij[r1][c0] * ij[r2][c2] - ij[r2][c0] * ij[r1][c2]) +
			ij[r0][c2] * (ij[r1][c0] * ij[r2][c1] - ij[r2][c0] * ij[r1][c1]); 
	}

	void adjoint(matrix_type &M) const {
		M.ij[0][0] =  minor( 1, 2, 3, 1, 2, 3);
		M.ij[0][1] = -minor( 0, 2, 3, 1, 2, 3);
		M.ij[0][2] =  minor( 0, 1, 3, 1, 2, 3);
		M.ij[0][3] = -minor( 0, 1, 2, 1, 2, 3);
		M.ij[1][0] = -minor( 1, 2, 3, 0, 2, 3);
		M.ij[1][1] =  minor( 0, 2, 3, 0, 2, 3);
		M.ij[1][2] = -minor( 0, 1, 3, 0, 2, 3);
		M.ij[1][3] =  minor( 0, 1, 2, 0, 2, 3);
		M.ij[2][0] =  minor( 1, 2, 3, 0, 1, 3);
		M.ij[2][1] = -minor( 0, 2, 3, 0, 1, 3);
		M.ij[2][2] =  minor( 0, 1, 3, 0, 1, 3);
		M.ij[2][3] = -minor( 0, 1, 2, 0, 1, 3);
		M.ij[3][0] = -minor( 1, 2, 3, 0, 1, 2);
		M.ij[3][1] =  minor( 0, 2, 3, 0, 1, 2);
		M.ij[3][2] = -minor( 0, 1, 3, 0, 1, 2);
		M.ij[3][3] =  minor( 0, 1, 2, 0, 1, 2);
	}

	scalar_type determinant() const {
		return  ij[0][0] * minor(1, 2, 3, 1, 2, 3) -
			ij[0][1] * minor(1, 2, 3, 0, 2, 3) +
			ij[0][2] * minor(1, 2, 3, 0, 1, 3) -
			ij[0][3] * minor(1, 2, 3, 0, 1, 2);
	}

	// вычислить обратную матрицу и записать ее в M
	// возвращает false если детерминант равен нулю
	bool inverse(matrix_type &M) const {
		scalar_type det=determinant();
		if(det == 0) return false;
		adjoint(M);
		M /= det;
		return true;
	}

	// инвертирует матрицу
	// возвращает false если детерминант равен нулю
	bool invert() {
		return matrix_type(*this).inverse(*this);
	}

	// записать в T транспонированную матрицу
	void transpose(matrix_type &tp) {
        	tp.ij[0][0] = ij[0][0];	tp.ij[0][1] = ij[1][0];	tp.ij[0][2] = ij[2][0]; tp.ij[0][3] = ij[3][0];
		tp.ij[1][0] = ij[0][1];	tp.ij[1][1] = ij[1][1];	tp.ij[1][2] = ij[2][1]; tp.ij[1][3] = ij[3][1];
		tp.ij[2][0] = ij[0][2];	tp.ij[2][1] = ij[1][2];	tp.ij[2][2] = ij[2][2]; tp.ij[2][3] = ij[3][2];
		tp.ij[3][0] = ij[0][3];	tp.ij[3][1] = ij[1][3];	tp.ij[3][2] = ij[2][3]; tp.ij[3][3] = ij[3][3];
	}

	// операции
	#if defined(D3DMATRIX_DEFINED)
	operator D3DMATRIX() const {
		D3DMATRIX result;
		for(size_type i=0; i<width; i++) {
			for(size_type j=0; j<height; j++) {
				result.m[i][j] = float(ij[i][j]);
			}
		}
		return result;
	}
	#endif

	matrix_type &operator *=(scalar_type k) {
		ij[0][0]*=k;	ij[0][1]*=k;	ij[0][2]*=k;	ij[0][3]*=k;
		ij[1][0]*=k;	ij[1][1]*=k;	ij[1][2]*=k;	ij[1][3]*=k;
		ij[2][0]*=k;	ij[2][1]*=k;	ij[2][2]*=k;	ij[2][3]*=k;
		ij[3][0]*=k;	ij[3][1]*=k;	ij[3][2]*=k;	ij[3][3]*=k;
		return *this;
	}

	matrix_type &operator /=(scalar_type k) {
		return *this *= 1/k;
	}

	matrix_type &operator *=(const matrix_type &m) {
		matrix_type n=*this;
		mul(*this,n,m);
		return *this;
	}

	point_type projected_column(size_type column_index) const {
		return point_type(ij[column_index][0], ij[column_index][1], ij[column_index][2]);
	}

	vec_type column(size_type column_index) const {
		return vec_type(ij[column_index][0], ij[column_index][1], ij[column_index][2],
			ij[column_index][3]);
	}

	point_type projected_row(size_type row_index) const {
		return point_type(ij[0][row_index], ij[1][row_index], ij[2][row_index]);
	}

	vec_type row(size_type row_index) const {
		return vec_type(ij[0][row_index], ij[1][row_index], ij[2][row_index],
			ij[3][row_index]);
	}

	bool decompose(point_type &scale,point_type &shear,point_type &rotate,point_type &translate,
		vec_type &persp) const;
};

template<int M,int N,class T> inline math::vec<N-1,T>
operator *(const math::vec<N-1,T> &left, const math::matrix<M,N,T> &right) {
	math::vec<N-1,T> result;
	mul(result,left,right);
	return result;
}

template<int M,int N,class T>
inline math::vec<N,T> operator *(const math::vec<N,T> &left, const math::matrix<M,N,T> &right) {
	math::vec<N,T> result;
	mul(result,left,right);
	return result;
}

template<int M,int N,int K,class T> inline math::matrix<N,K,T>
operator *(const math::matrix<M,N,T> &left,const math::matrix<N,K,T> &right) {
	math::matrix<N,K,T> result;
	mul(result,left,right);
	return result;
}

template<int M,int N,class T>
inline math::vec<N,T> &operator *=(math::vec<N,T> &left, const math::matrix<M,N,T> &right) {
	mul(left,math::vec<N,T>(left),right);
	return left;
}

template<class T> void
math::mul(math::vec<2,T> &result,const math::vec<2,T> &v,const math::matrix<3,3,T> &m) {
	math::vec<3,T> r;
	mul(r,v,m);
	result.x = r.x/r.z;
	result.y = r.y/r.z;
}

template<class T> void
math::mul(math::vec<3,T> &result,const math::vec<2,T> &v,const math::matrix<3,3,T> &m) {
	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0];
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1];
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2];
}

template<class T> void
math::mul(math::vec<3,T> &result,const math::vec<3,T> &v,const math::matrix<3,3,T> &m) {
	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z;
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z;
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z;
}

template<class T> void
math::mul(math::vec<3,T> &result,const math::vec<3,T> &v,const math::matrix<4,4,T> &m) {
	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z+m.ij[3][0];
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z+m.ij[3][1];
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z+m.ij[3][2];
}

template<class T> void
math::mul(math::vec<4,T> &result,const math::vec<3,T> &v,const math::matrix<4,4,T> &m) {
	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z+m.ij[3][0];
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z+m.ij[3][1];
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z+m.ij[3][2];
	result.w=m.ij[0][3]*v.x+m.ij[1][3]*v.y+m.ij[2][3]*v.z+m.ij[3][3];
}

template<class T> void
math::mul(math::vec<4,T> &result,const math::vec<4,T> &v,const math::matrix<4,4,T> &m) {
	result.x=m.ij[0][0]*v.x+m.ij[1][0]*v.y+m.ij[2][0]*v.z+m.ij[3][0]*v.w;
	result.y=m.ij[0][1]*v.x+m.ij[1][1]*v.y+m.ij[2][1]*v.z+m.ij[3][1]*v.w;
	result.z=m.ij[0][2]*v.x+m.ij[1][2]*v.y+m.ij[2][2]*v.z+m.ij[3][2]*v.w;
	result.w=m.ij[0][3]*v.x+m.ij[1][3]*v.y+m.ij[2][3]*v.z+m.ij[3][3]*v.w;
}

template<int M,int N,int K,class T> void
math::mul(math::matrix<M,K,T> &result,const math::matrix<M,N,T> &left,const math::matrix<N,K,T> &right) {
	int i,j,k;
	for(i=0; i<M; i++) {
		for(j=0; j<K; j++) {
			result.ij[i][j]=left.ij[i][0]*right.ij[0][j];
			for(k=1; k<N; k++) result.ij[i][j]+=left.ij[i][k]*right.ij[k][j];
		}
	}
}

