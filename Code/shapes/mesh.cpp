#include "mesh.h"
#include "../objloader.h"

#include <cmath>
#include <iostream>

using namespace std;

Hit Mesh::intersect(Ray const &ray)
{
	// Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    Triangle* triangle = nullptr;
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

void Mesh::addPolygon(Triangle* poly) {
    triangles.push_back(poly);
}

void Mesh::translate(Vector translation) {
    for (unsigned i = 0; i != triangles.size(); i++)
    {
        triangles[i]->translate(translation);
    }
}

void Mesh::scale(double factor) {
    for (unsigned i = 0; i != triangles.size(); i++)
    {
        triangles[i]->scale(factor);
    }
}

Color Mesh::colorAt(Point const &point) {
    return material.color;
}

Mesh::Mesh()
{}

Mesh::Mesh(std::string filename) {
    OBJLoader objLoader = OBJLoader(filename);
    vector<Vertex> vertices = objLoader.vertex_data();
    for (unsigned i = 0; i < vertices.size(); i += 3) {
        addPolygon(new Triangle(
            Point(vertices[i+0].x, vertices[i+0].y, vertices[i+0].z),
            Point(vertices[i+1].x, vertices[i+1].y, vertices[i+1].z),
            Point(vertices[i+2].x, vertices[i+2].y, vertices[i+2].z),
            Vector(vertices[i+0].nx, vertices[i+0].ny, vertices[i+0].nz),
            Vector(vertices[i+1].nx, vertices[i+1].ny, vertices[i+1].nz),
            Vector(vertices[i+2].nx, vertices[i+2].ny, vertices[i+2].nz)));
    }
}
