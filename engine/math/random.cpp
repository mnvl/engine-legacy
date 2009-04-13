
/*-------------------------------------------------------------------------
This random number generator originally appeared in "Toward a Universal 
Random Number Generator" by George Marsaglia and Arif Zaman. 
Florida State University Report: FSU-SCRI-87-50 (1987)

It was later modified by F. James and published in "A Review of Pseudo-
random Number Generators" 

THIS IS THE BEST KNOWN RANDOM NUMBER GENERATOR AVAILABLE.

(However, a newly discovered technique can yield a period of 10^600.
But that is still in the development stage.)

It passes ALL of the tests for random number generators and has a period 
of 2^144, is completely portable (gives bit identical results on all 
machines with at least 24-bit mantissas in the floating point 
representation). 
 
The algorithm is a combination of a Fibonacci sequence (with lags of 97
and 33, and operation "subtraction plus one, modulo one") and an 
"arithmetic sequence" (using subtraction).

On a Vax 11/780, this random number generator can produce a number in 
13 microseconds (FORTRAN version - JB). 
-------------------------------------------------------------------------*/

#include <precompiled.h>
#include <meta/typedefs.h>
#include <meta/throw_site.h>
#include <math/random.h>

math::random::random() {
	meta::s64 ij;
	int kl;

	QueryPerformanceCounter((LARGE_INTEGER *) &ij);
	kl=GetCurrentThreadId();

	set_seed(int(ij%ij_max), kl%kl_max);
}

math::random::~random() {
}

void math::random::set_seed(int ij,int kl) {
	int i, j, k, l, ii, jj, m;
	scalar s, t;

	if(ij<0 || ij>ij_max) throw std::runtime_error("The first random number seed must have a value between 0 and 31328.");
	if(kl<0 || kl>kl_max) throw std::runtime_error("The second seed must have a value between 0 and 30081.");

	i = (ij/177)%177 + 2;
	j = ij%177 + 2;
	k = (kl/169)%178 + 1;
	l = kl%169;

	for (ii=1; ii<=97; ii++) {
		s = 0.0;
		t = 0.5;
		for (jj=1; jj<=24; jj++) {
			m = (((i*j)%179)*k) % 179;
			i = j;
			j = k;
			k = m;
			l = (53*l + 1) % 169;
			if ((l*m)%64 >= 32) s += t;
			t *= 0.5;
		}
		u[ii] = s;
	}

	c = 362436.0 / 16777216.0;
	cd = 7654321.0 / 16777216.0;
	cm = 16777213.0 / 16777216.0;

	i97 = 97;
	j97 = 33;
}
