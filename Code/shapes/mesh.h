#ifndef MESH_H_
#define MESH_H_

#include "../object.h"
#include "triangle.h"
#include <vector>

class Mesh: public Object
{
    public:
        Mesh();
        Mesh(std::string filename);

        void addPolygon(Triangle* poly);

        void translate(Vector translation);
        void scale(double factor);

        virtual Hit intersect(Ray const &ray);
        virtual Color colorAt(Point const &point);

        std::vector<Triangle*> triangles;
};

#endif
