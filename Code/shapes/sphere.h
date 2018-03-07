#ifndef SPHERE_H_
#define SPHERE_H_

#include "../object.h"
#include "../image.h"

class Sphere: public Object
{
	Vector rotationAxis;
	double rotationAngle;
	double phiRotation = 0;
	bool rotation = false;

	public:
        Sphere(Point const &pos, double radius);

        virtual Hit intersect(Ray const &ray);
        virtual Color colorAt(Point const &point);
        void setRotation(Vector axis, double angle);

        Point const position;
        double const r;
};

#endif
