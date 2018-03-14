#include "raytracer.h"

#include "image.h"
#include "light.h"
#include "material.h"
#include "triple.h"
#include "objloader.h"

// =============================================================================
// -- Include all your shapes here ---------------------------------------------
// =============================================================================

#include "shapes/sphere.h"
#include "shapes/triangle.h"
#include "shapes/plane.h"
#include "shapes/quad.h"
#include "shapes/mesh.h"

// =============================================================================
// -- End of shape includes ----------------------------------------------------
// =============================================================================

#include "json/json.h"

#include <exception>
#include <fstream>
#include <iostream>

using namespace std;        // no std:: required
using json = nlohmann::json;

bool Raytracer::parseObjectNode(json const &node)
{
    ObjectPtr obj = nullptr;

// =============================================================================
// -- Determine type and parse object parametrers ------------------------------
// =============================================================================

    if (node["type"] == "sphere")
    {
        Point pos(node["position"]);
        double radius = node["radius"];
        Sphere* sphere = new Sphere(pos, radius);
        if (node.find("rotation") != node.end() && node.find("angle") != node.end()) {
            Vector rotation(node["rotation"]);
            double angle(node["angle"]);
            sphere->setRotation(rotation, angle);
        }
        obj = ObjectPtr(sphere);
    } else if (node["type"] == "triangle")
    {
        Point v0(node["v0"]);
        Point v1(node["v1"]);
        Point v2(node["v2"]);
        obj = ObjectPtr(new Triangle(v0, v1, v2));
    } else if (node["type"] == "quad")
    {
        Point v0(node["v0"]);
        Point v1(node["v1"]);
        Point v2(node["v2"]);
        Point v3(node["v3"]);
        obj = ObjectPtr(new Quad(v0, v1, v2, v3));
    } else if (node["type"] == "plane")
    {
        Point pos(node["position"]);
        Vector normal(node["normal"]);
        obj = ObjectPtr(new Plane(pos, normal));
    } else if (node["type"] == "mesh")
    {
        string model = node["model"];
        Vector translation(0,0,0);
        if (node.find("translation") != node.end()) {
            translation = Vector(node["translation"]);
        }
        Mesh* mesh = new Mesh(model);
        mesh->scale(node.value("scaling",1));
        mesh->translate(translation);
        obj = ObjectPtr(mesh);
    } else
    {
        cerr << "Unknown object type: " << node["type"] << ".\n";
    }

// =============================================================================
// -- End of object reading ----------------------------------------------------
// =============================================================================
    
    if (!obj)
        return false;

    // Parse material and add object to the scene
    obj->material = parseMaterialNode(node["material"]);
    scene.addObject(obj);
    
    return true;
}

Light Raytracer::parseLightNode(json const &node) const
{
    Point pos(node["position"]);
    Color col(node["color"]);
    return Light(pos, col);
}

Material Raytracer::parseMaterialNode(json const &node) const
{
    double ka = node["ka"];
    double kd = node["kd"];
    double ks = node["ks"];
    double n  = node["n"];
    double reflection = node["reflection"];

    string tex = node.value("texture", "");
    if (tex == "") {
        Color color(node["color"]);
        return Material(color, ka, kd, ks, n, reflection);
    } else {
        return Material(sceneDir+"/"+tex, ka, kd, ks, n, reflection);
    }
}

bool Raytracer::readScene(string const &ifname)
try
{
    // Read and parse input json file
    ifstream infile(ifname);
    if (!infile) throw runtime_error("Could not open input file for reading.");
    json jsonscene;
    infile >> jsonscene;

    sceneDir = ifname;
    sceneDir.erase(sceneDir.begin() + sceneDir.find_last_of('/'), sceneDir.end());

// =============================================================================
// -- Read your scene data in this section -------------------------------------
// =============================================================================

    Point eye(jsonscene["Eye"]);
    scene.setEye(eye);

    bool shadows = jsonscene.value("Shadows", true);
    scene.setShadows(shadows);

    int reflections = jsonscene.value("MaxRecursionDepth", 0);
    scene.setRecursionDepth(reflections);

    int factor = jsonscene.value("SuperSamplingFactor", 1);
    scene.setSuperSamplingFactor(factor);

    int res = jsonscene.value("Resolution", 400);
    setResolution(res);

    for (auto const &lightNode : jsonscene["Lights"])
        scene.addLight(parseLightNode(lightNode));

    unsigned objCount = 0;
    for (auto const &objectNode : jsonscene["Objects"])
        if (parseObjectNode(objectNode))
            ++objCount;

    cout << "Parsed " << objCount << " objects.\n";

// =============================================================================
// -- End of scene data reading ------------------------------------------------
// =============================================================================

    return true;
}
catch (exception const &ex)
{
    cerr << ex.what() << '\n';
    return false;
}

void Raytracer::setResolution(int res) {
    resolution = res;
}

void Raytracer::renderToFile(string const &ofname)
{
    // TODO: the size may be a settings in your file
    Image img(resolution, resolution);
    cout << "Tracing...\n";
    scene.render(img);
    cout << "Writing image to " << ofname << "...\n";
    img.write_png(ofname);
    cout << "Done.\n";
}
