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
    ObjectPtr obj2 = nullptr;

// =============================================================================
// -- Determine type and parse object parametrers ------------------------------
// =============================================================================

    if (node["type"] == "sphere")
    {
        Point pos(node["position"]);
        double radius = node["radius"];
        obj = ObjectPtr(new Sphere(pos, radius));
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
        Material material = parseMaterialNode(node["material"]);
        OBJLoader obj = OBJLoader(model);
        vector<Vertex> vertices = obj.vertex_data();
        for (uint i = 0; i < vertices.size(); i += 3) {
            // Hardcoded transformation
            // TODO: not hardcode the transformation
            Point v0 = Point(vertices[i+0].x*400+200, vertices[i+0].y*400+100, vertices[i+0].z*400);
            Vector n0 = Vector(vertices[i+0].nx, vertices[i+0].ny, vertices[i+0].nz);
            Point v1 = Point(vertices[i+1].x*400+200, vertices[i+1].y*400+100, vertices[i+1].z*400);
            Vector n1 = Vector(vertices[i+1].nx, vertices[i+1].ny, vertices[i+1].nz);
            Point v2 = Point(vertices[i+2].x*400+200, vertices[i+2].y*400+100, vertices[i+2].z*400);
            Vector n2 = Vector(vertices[i+2].nx, vertices[i+2].ny, vertices[i+2].nz);
            ObjectPtr obj = ObjectPtr(new Triangle(v0, v1, v2, n0, n1, n2));
            obj->material = material;
            scene.addObject(obj);
        }
        return true;
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
    
    if (obj2) {
		obj2->material = parseMaterialNode(node["material"]);
		scene.addObject(obj2);
	}
    
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
    Color color(node["color"]);
    double ka = node["ka"];
    double kd = node["kd"];
    double ks = node["ks"];
    double n  = node["n"];
    return Material(color, ka, kd, ks, n);
}

bool Raytracer::readScene(string const &ifname)
try
{
    // Read and parse input json file
    ifstream infile(ifname);
    if (!infile) throw runtime_error("Could not open input file for reading.");
    json jsonscene;
    infile >> jsonscene;

// =============================================================================
// -- Read your scene data in this section -------------------------------------
// =============================================================================

    Point eye(jsonscene["Eye"]);
    scene.setEye(eye);

    // TODO: add your other configuration settings here

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

void Raytracer::renderToFile(string const &ofname)
{
    // TODO: the size may be a settings in your file
    Image img(400, 400);
    cout << "Tracing...\n";
    scene.render(img);
    cout << "Writing image to " << ofname << "...\n";
    img.write_png(ofname);
    cout << "Done.\n";
}
