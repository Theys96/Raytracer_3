#ifndef PLANE_H_
#define PLANE_H_

#include "../object.h"

class Plane: public Object
{
    public:
        Plane(Point const &pos, Vector const &normal);

        virtual Hit intersect(Ray const &ray);

        Point const pos;
        Vector const norm;
};

#endif
