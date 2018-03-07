#ifndef SPHERE_H_
#define SPHERE_H_

#include "../object.h"
#include "../image.h"

class Sphere: public Object
{
	public:
        Sphere(Point const &pos, double radius);

        virtual Hit intersect(Ray const &ray);
        virtual Color colorAt(Point const &point);

        Point const position;
        double const r;
};

#endif
