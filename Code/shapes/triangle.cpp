#include "triangle.h"

#include <cmath>
#include <iostream>

using namespace std;

Hit Triangle::intersect(Ray const &ray)
{
	Vector S = ray.O - v0;
	Vector H = ray.D.cross(e2);
	Vector Q = S.cross(e1);

	double a = H.dot(e1);
	if (a == 0)
		return Hit::NO_HIT();
	
	double u = (S.dot(H))/a;
	double v = (ray.D.dot(Q))/a;

	if (u < 0 || v < 0 || u+v > 1) 
		return Hit::NO_HIT();
	
	double t = (e2.dot(Q))/a;
	Vector norm = ((1-u-v)*norm0) + (u*norm1) + (v*norm2);

	return Hit(t, norm);
}

Triangle::Triangle(Point const &v0, Point const &v1, Point const &v2)
:
    v0(v0),
    e1(v1 - v0),
    e2(v2 - v0)
{
	norm0 = e1.cross(e2).normalized();
	norm1 = norm0;
	norm2 = norm1;
}

Triangle::Triangle(
        	Point const &v0, Point const &v1, Point const &v2,
        	Vector norm0, Vector norm1, Vector norm2)
:
    v0(v0),
    e1(v1 - v0),
    e2(v2 - v0),
    norm0(norm0),
    norm1(norm1),
    norm2(norm2)
{
}
