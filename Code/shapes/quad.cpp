#include "quad.h"
#include "triangle.h"

#include <cmath>
#include <iostream>

using namespace std;

Hit Quad::intersect(Ray const &ray)
{
	// Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr triangle = nullptr;
    for (unsigned i = 0; i != triangles.size(); i++)
    {
        Hit hit(triangles[i]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            triangle = triangles[i];
        }
    }

    if (!triangle)
    	return Hit::NO_HIT();
	return min_hit;
}

Quad::Quad(Point const &v0, Point const &v1, Point const &v2, Point const &v3)
{
	// Relative points
	Point e1 = v1 - v0;
    Point e2 = v2 - v0;
    Point e3 = v3 - v0;

    // Normals
    Vector n1 = e1.cross(e2).normalized();
    Vector n3 = e2.cross(e3).normalized();
    Vector n2 = ((n1 + n3)/2).normalized(); //Average

    triangles.push_back(ObjectPtr(new Triangle(v0, v1, v2, n2, n1, n2)));
    triangles.push_back(ObjectPtr(new Triangle(v0, v2, v3, n2, n2, n3)));
}
