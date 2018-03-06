#ifndef QUAD_H_
#define QUAD_H_

#include "../object.h"
#include <vector>

class Quad: public Object
{
    public:
        Quad(Point const &v0, Point const &v1, Point const &v2, Point const &v3);

        virtual Hit intersect(Ray const &ray);

        std::vector<ObjectPtr> triangles;
};

#endif
