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

        void translate(Vector translation);
        void scale(double factor);

        virtual Hit intersect(Ray const &ray);
        virtual Color colorAt(Point const &point);

        Point v0;
        Point e1;
        Point e2;
        Vector norm0;
        Vector norm1;
        Vector norm2;
};

#endif
