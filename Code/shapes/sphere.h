#ifndef SPHERE_H_
#define SPHERE_H_

#include "../object.h"
#include "../image.h"

class Sphere: public Object
{
	Image texture;
    bool usingTexture = false;

    public:
        Sphere(Point const &pos, double radius);

        virtual Hit intersect(Ray const &ray);
        virtual Color colorAt(Point const &point);
        void setTexture(std::string path);

        Point const position;
        double const r;
};

#endif
