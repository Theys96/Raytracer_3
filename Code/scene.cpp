#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <cmath>
#include <limits>
#include <iostream>

using namespace std;

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

Color Scene::trace(Ray const &ray, bool specularOnly, int reflection)
{
    // Find hit object and distance
    ObjectPtr obj = nullptr;
    Hit min_hit = findMinHit(ray, &obj);

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material material = obj->material;          //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector
    Color materialColor = obj->colorAt(hit);

    Color I_d;
    Color I_s;
    ObjectPtr shadowObj;
    for (int i = 0; i < lights.size(); i++) {

        Vector light = lights[i]->position - hit;
        Vector L = (light).normalized();

        bool castShadow = false;
        if (shadows) {
            Ray lightRay(hit, L);
            Hit shadowHit = findMinHit(lightRay, &shadowObj);
            castShadow = shadowHit.t < light.length();
        }

        if (!castShadow) {
            Vector R = 2*(N.dot(L))*N - L;
            I_d += lights[i]->color * fmax(0, L.dot(N));
            I_s += lights[i]->color * pow(fmax(0, R.dot(V)), material.n);
        }
    }

    if (reflection > 0) {
        Vector O = 2*(N.dot(V))*N - V;
        I_s += trace(Ray(hit, O), false, reflection-1);
    }

    I_d *= material.kd;
    I_s *= material.ks;


    Color I = material.ka+I_d;

    return I_s + (specularOnly ? Triple(0,0,0) : I * materialColor);
}

Color Scene::renderPixel(int x, int y, int w, int h) {
    std::vector<Color> samples;

    double base = 1/(double)(2*superSamplingFactor);
    double step = 1/(double)superSamplingFactor;
    for (int i = 1; i <= superSamplingFactor; i++) {
        for (int j = 1; j <= superSamplingFactor; j++) {
            Point samplePoint(x + base + i*step, h - 1 - y + base + j*step, 0);
            Ray ray(eye, (samplePoint - eye).normalized());
            Color col = trace(ray, false, recursionDepth);
            col.clamp();
            samples.push_back(col);
        }
    }

    Color col(0,0,0);
    for (int i = 0; i < samples.size(); i++) {
        col += samples[i];
    }
    col /= samples.size();

    return col;
}

void Scene::render(Image &img)
{
    unsigned w = img.width();
    unsigned h = img.height();
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
            img(x, y) = renderPixel(x, y, w, h);
        }
        cout << y << " ";
        if ((y+1)%10 == 0) {
            cout << "\n";
        }
    }
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
