#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../object.h"

class Triangle: public Object
{
    public:
        Triangle(Point const &v0, Point const &v1, Point const &v2);
        Triangle(
        	Point const &v0, Point const &v1, Point const &v2,
        	Vector norm0, Vector norm1, Vector norm2);

        virtual Hit intersect(Ray const &ray);

        Point const v0;
        Point const e1;
        Point const e2;
        Vector norm0;
        Vector norm1;
        Vector norm2;
};

#endif
