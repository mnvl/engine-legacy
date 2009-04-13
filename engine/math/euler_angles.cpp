
#include <precompiled.h>
#include <math/euler_angles.h>

template<class T> char const math::euler_angles<T>::safe[] = "\000\001\002\000";
template<class T> char const math::euler_angles<T>::next[] = "\001\002\000\001";

template class math::euler_angles<>;

template<class T> void
math::euler_angles<T>::load(matrix_type const &M, order_type ord) {
	order = ord;

	axis_type i,j,k,h;
	parity_type n;
	repetition_type s;
	frame_type f;
	unpack_order(order, f, s, n, i, j, k, h);

	if (s == REPETITION_YES) {
		scalar_type sy = sqrt(M.ij[i][j]*M.ij[i][j] + M.ij[i][k]*M.ij[i][k]);
		if (sy > 16*std::numeric_limits<math::scalar>::epsilon()) {
			x = atan2(M.ij[i][j], M.ij[i][k]);
			y = atan2(sy, M.ij[i][i]);
			z = atan2(M.ij[j][i], -M.ij[k][i]);
		} else {
			x = atan2(-M.ij[j][k], M.ij[j][j]);
			y = atan2(sy, M.ij[i][i]);
			z = 0;
		}
	} else {
		scalar_type cy = sqrt(M.ij[i][i]*M.ij[i][i] + M.ij[j][i]*M.ij[j][i]);
		if (cy > 16*std::numeric_limits<math::scalar>::epsilon()) {
			x = atan2(M.ij[k][j], M.ij[k][k]);
			y = atan2(-M.ij[k][i], cy);
			z = atan2(M.ij[j][i], M.ij[i][i]);
		} else {
			x = atan2(-M.ij[j][k], M.ij[j][j]);
			y = atan2(-M.ij[k][i], cy);
			z = 0;
		}
	}
	if (n == PARITY_ODD) {
		x = -x;
		y = -y;
		z = -z;
	}
	
	if (f == FRAME_ROTATED) {
		scalar_type t = x;
		x = z;
		z = t;
	}
}

template<class T> void
math::euler_angles<T>::load(quaternion_type const &Q, order_type ord) {
	matrix_type M;
	M.rotation(Q);

	load(M, ord);
}

template<class T> void
math::euler_angles<T>::save(matrix_type &M) const {
	euler_angles<T> ea = *this;
	scalar_type ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;

	axis_type i,j,k,h;
	parity_type n;
	repetition_type s;
	frame_type f;
	unpack_order(order, f, s, n, i, j, k, h);

	if(f == FRAME_ROTATED)
		std::swap(ea.x, ea.z);

	if(n == PARITY_ODD) {
		ea.x = -ea.x;
		ea.y = -ea.y;
		ea.z = -ea.z;
	}

	ti = ea.x;	tj = ea.y;	th = ea.z;
	ci = cos(ti); 	cj = cos(tj); 	ch = cos(th);
	si = sin(ti); 	sj = sin(tj); 	sh = sin(th);
	cc = ci*ch; 	cs = ci*sh; 	sc = si*ch; ss = si*sh;

	if(s == REPETITION_YES) {
		M.ij[i][i] = cj;       	M.ij[i][j] =  sj*si;    	M.ij[i][k] =  sj*ci;
		M.ij[j][i] = sj*sh;  	M.ij[j][j] = -cj*ss+cc; 	M.ij[j][k] = -cj*cs-sc;
		M.ij[k][i] = -sj*ch; 	M.ij[k][j] =  cj*sc+cs; 	M.ij[k][k] =  cj*cc-ss;
	} else {
		M.ij[i][i] = cj*ch; 	M.ij[i][j] = sj*sc-cs; 		M.ij[i][k] = sj*cc+ss;
		M.ij[j][i] = cj*sh; 	M.ij[j][j] = sj*ss+cc; 		M.ij[j][k] = sj*cs-sc;
		M.ij[k][i] = -sj;	M.ij[k][j] = cj*si;    		M.ij[k][k] = cj*ci;
	}
}

template<class T> void
math::euler_angles<T>::save(quaternion_type &Q) const {
	euler_angles<T> ea = *this;
	scalar_type a[3], ti, tj, th, ci, cj, ch, si, sj, sh, cc, cs, sc, ss;

	axis_type i,j,k,h;
	parity_type n;
	repetition_type s;
	frame_type f;
	unpack_order(order, f, s, n, i, j, k, h);

	if(f == FRAME_ROTATED)
		std::swap(ea.x, ea.z);

	if(n == PARITY_ODD)
		ea.y = -ea.y;

	ti = ea.x*0.5f; tj = ea.y*0.5f; th = ea.z*0.5f;
	ci = cos(ti);   cj = cos(tj);   ch = cos(th);
	si = sin(ti);   sj = sin(tj);   sh = sin(th);
	cc = ci*ch;     cs = ci*sh;     sc = si*ch;     ss = si*sh;

	if (s == REPETITION_YES) {
		a[i] = cj*(cs + sc);	/* Could speed up with */
		a[j] = sj*(cc + ss);	/* trig identities. */
		a[k] = sj*(cs - sc);
		Q.w = cj*(cc - ss);
	} else {
		a[i] = cj*sc - sj*cs;
		a[j] = cj*ss + sj*cc;
		a[k] = cj*cs - sj*sc;
		Q.w = cj*cc + sj*ss;
	}

	if(n == PARITY_ODD)
		a[j] = -a[j];

	Q.x = a[0];
	Q.y = a[1];
	Q.z = a[2]; 
}
