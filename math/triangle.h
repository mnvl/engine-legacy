#pragma once

#include <math/scalar.h>
#include <math/vec.h>
#include <math/ray.h>
#include <math/line.h>
#include <math/plane.h>
#include <math/aabb.h>
#include <math/sphere.h>
#include <math/matrix.h>

namespace math {

template<int N,class T=scalar> class triangle;

template<class T>
class triangle<3,T> {
public:
    typedef T scalar_type;
    typedef std::numeric_limits<scalar_type> limits_type;
    typedef size_t size_type;
    static const size_type arity = 3;
    typedef vec<arity,scalar_type> vec_type;
    typedef ray<arity,scalar_type> ray_type;
    typedef line<arity,scalar_type> line_type;
    typedef plane<scalar_type> plane_type;
    typedef aabb<arity,scalar_type> aabb_type;
    typedef sphere<arity, scalar_type> sphere_type;
    typedef matrix<arity+1,arity+1,scalar_type> matrix_type;
    typedef triangle<arity,scalar_type> triangle_type;

    vec_type A, B, C;

    triangle() {
    }

    triangle(const vec_type &a,const vec_type &b,const vec_type &c):
        A(a),
        B(b),
        C(c)
    {
    }

    void construct(const vec_type &a,const vec_type &b,const vec_type &c) {
        A=a;
        B=b;
        C=c;
    }

    // Graphic Gems III, ed. David Kirk. V.1 Triangles Revisited, pp. 215-218
    // the normal of the triangle
    vec_type get_normal() const {
        return normalize((B-A)^(C-B));
    }
	
    // the AABB of the triangle
    void get_aabb(const aabb_type &locus) const {
        locus.min = locus.max = A;
        locus.extend(B);
        locus.extend(C);
    }
	
    // the plane of the triangle
    plane_type get_plane() const {
        return plane_type(A,get_normal());
    }
	
    // the perimeter of the triangle
    scalar_type perimeter() const {
        return (B-A).length()+(C-B).length()+(A-C).length();
    }
	
    // the area of the triangle
    scalar_type area() const {
        return scalar_type(0.5)*((A^B) + (B^C) + (C^A)).length();
    }
	
    // the centre of gravity, i.e. the concurrence of the medians
    vec_type cog() const {
        return (A + B + C) / scalar_type(3);
    }
	
    // the orthocenter, i.e. the concurrence of the altitudes
    vec_type orthocenter() const {
        vec_type a = B - C;
        vec_type b = C - A;
        vec_type c = A - B;
	
        scalar_type d1 = -(b & c);
        scalar_type d2 = -(c & a);
        scalar_type d3 = -(a & b);
	
        scalar_type e1 = d2 * d3;
        scalar_type e2 = d3 * d1;
        scalar_type e3 = d1 * d2;
	
        return (e1 * A + e2 * B + e3 * C) / (e1 + e2 + e3);
    }
	
    // the circumcenter, i.e. the concurrence of the perpendicular
    // bisectors to the sides
    vec_type circumcenter() const {
        return scalar_type(0.5)*(scalar_type(3)*cog() - orthocenter());
    }
	
    // the radius of the circumcircle, i.e. the circle that passes
    // through the three vertices
    scalar_type circumcircle_radius() const {
        vec_type a = B - C;
        vec_type b = C - A;
        vec_type c = A - B;
	
        scalar_type d1 = -(b & c);
        scalar_type d2 = -(c & a);
        scalar_type d3 = -(a & b);
	
        scalar_type e1 = d2 * d3;
        scalar_type e2 = d3 * d1;
        scalar_type e3 = d1 * d2;
	
        scalar_type e = e1 + e2 + e3;
	
        return scalar_type(0.5)*sqrt((a.length_sq()+b.length_sq()+c.length_sq())/e);
    }
	
    sphere_type get_circumcircle() const {
        return sphere_type(circumcenter(), circumcircle_radius());
    }
	
    // the incenter, i.e. the concurrence of the angle bisectors
    vec_type incenter() const {
        vec_type a = B - C;
        vec_type b = C - A;
        vec_type c = A - B;
	
        scalar_type ab = a.length();
        scalar_type bc = b.length();
        scalar_type ca = c.length();
	
        return (ab*A+bc*B+ca*C)/(ab+bc+ca);
    }
	
    // the radius of the incircle, i.e. the circle inscribed in the
    // triangle, tangent to the three sides
    scalar_type incircle_radius() const {
        vec_type a = B - C;
        vec_type b = C - A;
        vec_type c = A - B;
	
        scalar_type ab=a.length();
        scalar_type bc=b.length();
        scalar_type ca=c.length();
	
        return area()/(ab+bc+ca);
    }
	
    sphere_type get_incircle() const {
        return sphere_type(incenter(), incircle_radius());
    }
	
    // the excenters and the radii of the excircles, i.e. the circles
    // tangent to one side of the triangle and to the extensions of the
    // other two sides
    vec_type excenter_A() const {
        vec_type a = B - C;
        vec_type b = C - A;
        vec_type c = A - B;
	
        scalar_type ab=a.length();
        scalar_type bc=b.length();
        scalar_type ca=c.length();
	
        return ((ab+bc+ca)*incenter()-ab*A)/(bc+ca);
    }
	
    scalar_type excircle_A_radius() const {
        vec_type b = C - A;
        vec_type c = A - B;
	
        scalar_type bc=b.length();
        scalar_type ca=c.length();
	
        return area()/(bc+ca);
    }
	
    vec_type excenter_B() const {
        vec_type a = B - C;
        vec_type b = C - A;
        vec_type c = A - B;
	
        scalar_type ab=a.length();
        scalar_type bc=b.length();
        scalar_type ca=c.length();
	
        return ((ab+bc+ca)*incenter()-bc*B)/(ab+ca);
    }
	
    scalar_type excircle_B_radius() const {
        vec_type a = B - C;
        vec_type c = A - B;
	
        scalar_type ab=a.length();
        scalar_type ca=c.length();
	
        return area()/(ab+ca);
    }
	
    vec_type excenter_C() const {
        vec_type a = B - C;
        vec_type b = C - A;
        vec_type c = A - B;
	
        scalar_type ab=a.length();
        scalar_type bc=b.length();
        scalar_type ca=c.length();
	
        return ((ab+bc+ca)*incenter()-ca*C)/(ab+bc);
    }
	
    scalar_type excircle_C_radius() const {
        vec_type a = B - C;
        vec_type b = C - A;
			
        scalar_type ab=a.length();
        scalar_type bc=b.length();
	
        return area()/(ab+bc);
    }
	
    // the centre and the radius of the Feuerbach circle (nine-point
    // circle), i.e. the circle that passes through the three feet of
    // the altitudes and the middle points of the sides
    vec_type feuerbach_center() const {
        return scalar_type(0.5)*(circumcenter()+orthocenter());
    }
	
    scalar_type feuerbach_radius() const {
        return scalar_type(0.5)*circumcircle_radius();
    }

    // transform triangle by matrix
    void transform(const matrix_type &tf) {
        mul(A, vec_type(A), tf);
        mul(B, vec_type(B), tf);
        mul(C, vec_type(C), tf);
    }

    // only for points on triangles plane
    bool contains_on_plane(const vec_type &v) const {
        vec_type BA = B - A;
        scalar_type s1 = (v - A) & BA;
        if(s1<0 || s1>BA.length_sq()) return false;

        vec_type CB = C - B;
        scalar_type s2 = (v - B) & CB;
        if(s2<0 || s2>CB.length_sq()) return false;

        vec_type AC = A - C;
        scalar_type s3 = (v - C) & AC;
        return s3>=0 && s3<=AC.length_sq();
    }

	bool contains(vec_type const &v) const {
		return get_plane().contains(v) && contains_on_plane(v);
	}

    bool trace(const ray_type &r,scalar_type &t,vec_type &v,scalar_type t_min = 0, scalar_type t_max = 1) const {
        return get_plane().trace(r,t,v,t_min,t_max) && contains_on_plane(v);
    }

    bool trace(const ray_type &r,scalar_type t_min = 0,scalar_type t_max = 1) const {
        scalar_type t;
        vec_type v;
        return trace(r,t,v,t_min,t_max);
    }

    bool intersects(const triangle_type &t) const {
        line_type ln;
        return intersect(t, ln);
    }

    bool intersect(const triangle_type &t,line_type &ln) const {
        ray_type r;
        if(!get_plane().intersect(t.get_plane(),r)) return false;

        vec_type v[2];
        size_type v_size = 0;
        line_type a(A,B), b(B,C), c(C,A);
	
        if(a.intersect(r,v[v_size])) v_size++;
        if(b.intersect(r,v[v_size])) v_size++;
        if(v_size<2 && c.intersect(r,v[v_size])) v_size++; 
	
        if(v_size<2) return false;
	
        ln.construct(v[0],v[1]);
        return true;
    }
	
    // найти время через которое столкнутся два движущихся треугольника
    // и точку столкновения
    // tri1 -- данный треугольник
    // vel0 -- скорость настоящего треугольника
    // vel1 -- скорость треугольника tri1
    // t -- время через которое треугольники столкнутся
    // cp -- точка столкновения
    // cn -- нормаль столкновения
    // t_min, t_max -- минимальное и максимальное время расчета
    bool collide(const triangle_type &tri1,const vec_type &vel0,const vec_type &vel1,
        scalar_type &t,vec_type &cp,vec_type &cn,scalar_type t_min = 0, scalar t_max = 1) const
    {
        vec_type vel = vel1 - vel0, n0 = get_normal(), n1 = tri1.get_normal(), cp0;
        scalar_type a_n0 = -vel & n0, a_n1 = vel & n1, t0;
	
        t = limits_type::max();
	
        if(abs(a_n1) > 0) {
            t0 = ((A - tri1.A) & n1) / a_n1;
            cp0 = A - vel * t0;
            if(t_min <= t0 && t0 <= t_max && tri1.contains(cp0)) {
                t = t0;
                cp = A + vel0 * t;
                cn = n1;
            }
	
            t0 = ((B - tri1.A) & n1) / a_n1;
            cp0 = B - vel * t0;
            if(t_min <= t0 && t0 <= t_max && t0 < t && tri1.contains(cp0)) {
                t = t0;
                cp = B + vel0 * t;
                cn = n1;
            }
	
            t0 = ((C - tri1.A) & n1) / a_n1;
            cp0 = C - vel * t0;
            if(t_min <= t0 && t0 <= t_max && t0 < t && tri1.contains(cp0)) {
                t = t0;
                cp = C + vel0 * t;
                cn = n1;
            }
        }
	
        if(abs(a_n0) > 0) {
            t0 = ((tri1.A - A) & n0) / a_n0;
            cp0 = tri1.A + vel * t0;
            if(t_min <= t0 && t0 <= t_max && t0 < t && contains(cp0)) {
                t = t0;
                cp = tri1.A + vel0 * t;
                cn = n0;
            }
	
            t0 = ((tri1.B - A) & n0) / a_n0;
            cp0 = tri1.B + vel * t0;
            if(t_min <= t0 && t0 <= t_max && t0 < t && contains(cp0)) {
                t = t0;
                cp = tri1.B + vel0 * t;
                cn = n0;
            }
	
            t0 = ((tri1.C - A) & n0) / a_n0;
            cp0 = tri1.C + vel * t0;
            if(t_min <= t0 && t0 <= t_max && t0 < t && contains(cp0)) {
                t = t0;
                cp = tri1.C + vel0 * t;
                cn = n0;
            }
        }

        return t != limits_type::max();
    }
};

}

