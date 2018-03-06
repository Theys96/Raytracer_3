#include "plane.h"

#include <cmath>
#include <iostream>

using namespace std;

Hit Plane::intersect(Ray const &ray)
{
    //return Hit::NO_HIT();

    double t = (pos - ray.O).dot(norm)/(norm.dot(ray.D));
    if (t <= 0) {
    	return Hit::NO_HIT();
    }

    return Hit(t,norm);
}

Plane::Plane(Point const &pos, Vector const &normal)
:
    pos(pos),
    norm(normal)
{}
