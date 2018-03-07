#ifndef SCENE_H_
#define SCENE_H_

#include "light.h"
#include "object.h"
#include "triple.h"

#include <vector>

// Forward declerations
class Ray;
class Image;

class Scene
{
    std::vector<ObjectPtr> objects;
    std::vector<LightPtr> lights;   // no ptr needed, but kept for consistency
    Point eye;
    bool shadows = false;
    int recursionDepth = 0;
    int superSamplingFactor = 1;

    public:

        // trace a ray into the scene and return the color
        Color trace(Ray const &ray, int reflection);
        Hit findMinHit(Ray const &ray, ObjectPtr* obj);
        bool isBlocked(Point point, Vector direction, double maxDistance);

        // render the scene to the given image
        void render(Image &img);
        Color renderPixel(int x, int y, int w, int h);

        void addObject(ObjectPtr obj);
        void addLight(Light const &light);
        void setEye(Triple const &position);
        void setShadows(bool const &s);
        void setRecursionDepth(int const &depth);
        void setSuperSamplingFactor(int const &factor);

        unsigned getNumObject();
        unsigned getNumLights();
};

#endif
