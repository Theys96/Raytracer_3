#include "sphere.h"

#include <cmath>
#include <iostream>

using namespace std;

Hit Sphere::intersect(Ray const &ray)
{
    // Hit calculation
    Vector distance = ray.O - position;
    double b = distance.dot(ray.D);
    double d = b*b - distance.length_2() + r*r;
    if (d < 0) {
        return Hit::NO_HIT();
    }
    double t = -b-sqrt(d);
    if (t <= 0) {
        return Hit::NO_HIT();
    }

    // Normal calculation
    Vector N = (ray.at(t) - position).normalized();

    return Hit(t,N);
}

Sphere::Sphere(Point const &pos, double radius)
:
    position(pos),
    r(radius)
{}

Color Sphere::colorAt(Point const &point) {
    if (material.usingTexture) {
        Vector radius = point - position;
        if (rotation) {
            // Rodrigues' rotation formula
            radius = 
                cos(rotationAngle)*radius +
                sin(rotationAngle)*(rotationAxis.cross(radius)) +
                (1 - cos(rotationAngle))*(rotationAxis.dot(radius))*rotationAxis;
        }
        double theta = acos(radius.z/r);
        double phi = atan2(radius.y, radius.x) - phiRotation;

        while (phi < 0) phi += 2*M_PI;
        
        double u = phi/(2*M_PI);
        double v = theta/M_PI;
        
        return material.texture->colorAt(u,v);
    }
    return material.color;
}

void Sphere::setRotation(Vector axis, double angle) {
    rotationAxis = axis.normalized().cross(Vector(0,0,1));
    rotationAngle = rotationAxis.length();
    rotationAxis.normalize();
    phiRotation = angle/180*M_PI;
    rotation = true;
}
