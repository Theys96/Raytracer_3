#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <cmath>
#include <limits>
#include <iostream>

using namespace std;

// The "viewport" of the image is always 400x400.
// E.g. it stretches from (0,0,0), top-left to
// (400,400,0), bottom-right.
// However, the resolution of this viewport can be higher
// or lower if defined as such in the source file.
#define VIEWPORT_SIZE 400

Hit Scene::findMinHit(Ray const &ray, ObjectPtr* obj) {
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    for (unsigned idx = 0; idx != objects.size(); ++idx)
    {
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t && hit.t > 0.00000001)
        {
            min_hit = hit;
            *obj = objects[idx];
        }
    }
    return min_hit;
}

/*
 * Checks if a point is block is blocked in a certain direction,
 * up to a certain distance.
 */
bool Scene::isBlocked(Point point, Vector direction, double maxDistance) {
    ObjectPtr shadowObj;
    Ray lightRay(point, direction);
    Hit shadowHit = findMinHit(lightRay, &shadowObj);
    return shadowHit.t < maxDistance;
}

Color Scene::trace(Ray const &ray, int reflection)
{
    // Find hit object and distance
    ObjectPtr obj = nullptr;
    Hit min_hit = findMinHit(ray, &obj);

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material material = obj->material;     // The hit objects material
    Point hit = ray.at(min_hit.t);         // The hit point
    Vector N = min_hit.N;                  // The normal at hit point
    Vector V = -ray.D;                     // The view vector
    Color  I_d;                            // Diffuse light intensity, to be calculated
    Color  I_s;                            // Specular light intensity, to be calculated
    Color  I_r;                            // Light coming from reflection
    double I_a = material.ka;              // Ambient light intensity

    /* Lights calculation */
    for (int i = 0; i < lights.size(); i++) {
        Vector light = lights[i]->position - hit;
        Vector L = (light).normalized();
        if (!shadows || !isBlocked(hit, L, light.length())) {
            Vector R = 2*(N.dot(L))*N - L;
            I_d += lights[i]->color * fmax(0, L.dot(N));
            I_s += lights[i]->color * pow(fmax(0, R.dot(V)), material.n);
        }
    }

    /* Reflection calculation */
    if (reflection > 0 && material.reflection > 0) {
        Vector O = 2*(N.dot(V))*N - V;
        I_r = trace(Ray(hit, O), reflection-1);
    }

    /* Final combination */
    I_d *= material.kd;
    I_s *= material.ks;
    Color I = I_a + I_d;
    return I_s + I * obj->colorAt(hit) + material.reflection*I_r;
}

Color Scene::renderPixel(double x, double y, double pixelSize) {
    std::vector<Color> samples;
    
    double base = pixelSize/(double)(2*superSamplingFactor);
    double step = pixelSize/(double)superSamplingFactor;
    for (int i = 1; i <= superSamplingFactor; i++) {
        for (int j = 1; j <= superSamplingFactor; j++) {
            /* Raytracing a single sub-pixel */
            Point samplePoint(x + base + i*step, y + base + j*step, 0);
            Ray ray(eye, (samplePoint - eye).normalized());
            Color col = trace(ray, recursionDepth);
            col.clamp();
            samples.push_back(col);
        }
    }

    /* Find average pixel */
    Color col(0,0,0);
    for (int i = 0; i < samples.size(); i++) {
        col += samples[i];
    }
    return col / samples.size();
}

void Scene::render(Image &img)
{
    unsigned w = img.width();
    unsigned h = img.height();
    unsigned resolution = h;
    for (unsigned y = 0; y < h; ++y)
    {   
        // #pragma omp parallel for
        for (unsigned x = 0; x < w; ++x)
        {
            // This will stretch the picture if it is not a square
            // Transforming the pixels to in-viewport pixels
            // e.g. [0,resolution] -> [0,400]
            img(x, y) = renderPixel(
                (double)x/resolution*VIEWPORT_SIZE, 
                VIEWPORT_SIZE - 1 - (double)y/resolution*VIEWPORT_SIZE, 
                VIEWPORT_SIZE/(double)resolution
            );
        }
        if (y%100 == 0) cout << y/100 << "\t" << std::flush;
    }
    cout << "\n";
}

// --- Misc functions ----------------------------------------------------------

void Scene::addObject(ObjectPtr obj)
{
    objects.push_back(obj);
}

void Scene::addLight(Light const &light)
{
    lights.push_back(LightPtr(new Light(light)));
}

void Scene::setEye(Triple const &position)
{
    eye = position;
}

void Scene::setShadows(bool const &s)
{
    shadows = s;
}

void Scene::setRecursionDepth(int const &depth)
{
    recursionDepth = depth;
}

void Scene::setSuperSamplingFactor(int const &factor) 
{
    superSamplingFactor = factor;
}

unsigned Scene::getNumObject()
{
    return objects.size();
}

unsigned Scene::getNumLights()
{
    return lights.size();
}
