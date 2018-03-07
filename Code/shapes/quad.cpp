#include "quad.h"
#include "triangle.h"

#include <cmath>
#include <iostream>

using namespace std;

Hit Quad::intersect(Ray const &ray)
{
	return mesh.intersect(ray);
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

    mesh.addPolygon(new Triangle(v0, v1, v2, n2, n1, n2));
    mesh.addPolygon(new Triangle(v0, v2, v3, n2, n2, n3));
}

Color Quad::colorAt(Point const &point) {
    return material.color;
}
