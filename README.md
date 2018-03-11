# Introduction to Computer Graphics - Raytracer assignment 2

In this assignment, we did the following.

* **Implemented shadows as an option in the scene source file. Default is true**.
The algorithm simply shoots another ray in the direction of the light source from the hit-point. If there is a hit at a lower distance than the light, diffuse and specular light is omitted and not emitted.

* **Implemented reflection. The option MaxRecursionDepth in the source file determines the degree of reflection and default is 0, no reflection.**
Again, the algorithm shoot another ray from the hit-point, this time in the direction of the reflected view-vector. It records the color (if any) at the end of the ray and adds it to the specular light intensity.

* **Implemented anti-aliasing. Again, this is optional and the option SuperSamplingFactor can be set for example to 4 for 4x4 anti-aliasing. Default is 1 (no anti-aliasing).**
The algorithm has been extended to compute for a single pixel a variable amount of from-eye-rays or subpixels. These are divided as suggested in the assignment and averaged to get the color of the pixel. The anti-aliasing seems to work rather well and increase in image quality can be seen until around 8x8.

* **Implemented output picture resolution as an option in the scene source file. The viewport is always 400x400, but the resolution of the output picture can be set although default is 400 for a 400x400 output picture.**
Similarly to anti-aliasing, we extended the accepted JSON-language to accept a resolution number such that the output picture will be of size (resolution)x(resolution). If a higher (or lower) resolution than 400 is in fact chosen, the virtual pixels are chosen denser such that, for example, 1000x1000 pixels are a condensed in our viewport which still always stretches from (0,0,0) to (400,400,0). This works well together with anti-aliasing. 

* **Implemented texturing spheres using Miller projection from a PNG texture file.**
The texturing of spheres has been done using the algorithm from the book, which basically calculates the hit-point on the sphere to angles from the center of the sphere which can in turn be translated to coordinates in the texture map.

* **Implemented rotating (textured) spheres.**
We kept the rotation method as provided in the example source files. First, the initial position vector on the sphere is rotated by a rotation that is acquired by trying to rotate the (0,0,1) point on the sphere to a given vector. In our implementation, we did this by means of a cross product which gives us both an axis (the resulting vector, normalized) and an angle (the length of the resulting vector). We then use Rodrigues' rotation formula to execute this rotation. Then, we also rotate the texture with a given angle around the new north-south axis which can be simply done by subtracting the angle from our phi (=atan2(Δy, Δx)) angle, used for finding the texture-coordinates.

* Did a lot of refactoring on the existing code.

## Raytracer C++ framework

This is a C++ framework for a raytracer. It is created for the Computer
Science course "Introduction to Computer Graphics" taught at the
University of Groningen.

This framework is inspired by and uses (some) code of the raytracer framework of
Bert Freudenberg which unfortunately is no longer available.

## Compiling the code

We provided a [CMakeLists.txt](CMakeLists.txt) file to be used with `cmake`.
This can be used in the following way (on the command line/shell):
```
# cd into the main directory of the framework
mkdir build   # create a new build directory
cd build      # cd into the directory
cmake ..      # creates a Makefile
# Comipiling the code can then be done with:
make
# or
make -j4      # replacing 4 with the number of cores of your pc
```
**Note!** After adding new `.cpp` files (when adding new shapes)
`cmake ..` needs to be called again or you might get linker errors.

## Running the Raytracer
After compilation you should have the `ray` executable.
This can be used like this:
```
./ray <path to .json file> [output .png file]
# when in the build directory:
./ray ../Scenes/scene01.json
```
Specifying an output is optional and by default an image will be created in
the same directory as the source scene file with the `.json` extension replaced
by `.png`.

## Description of the included files

### Scene files
* `Scenes/*.json`: Scene files are structured in JSON. If you have never
    worked with json, please see [here](https://en.wikipedia.org/wiki/JSON#Data_types,_syntax_and_example)
    or [here](https://www.json.org/).

    Take a look at the provided example scenes for the general structure.
    You are free (and encouraged) to define your own scene files later on.

### The raytracer source files (Code directory)

* `main.cpp`: Contains main(), starting point. Responsible for parsing
    command-line arguments.

* `raytracer.cpp/.h`: Raytracer class. Responsible for reading the scene
    description, starting the raytracer and writing the result to an image file.

* `scene.cpp/.h`: Scene class. Contains code for the actual raytracing.

* `image.cpp/.h`: Image class, includes code for reading from and writing to PNG
    files.

* `light.h`: Light class. Plain Old Data (POD) class. Colored light at a
    position in the scene.

* `ray.h`: Ray class. POD class. Ray from an origin point in a direction.

* `hit.h`: Hit class. POD class. Intersection between an `Ray` and an `Object`.

* `object.h`: virtual `Object` class. Represents an object in the scene.
    All your shapes should derive from this class. See

* `shapes (directory/folder)`: Folder containing all your shapes.

* `sphere.cpp/.h (inside shapes)`: Sphere class, which is a subclass of the
    `Object` class. Represents a sphere in the scene.

* `example.cpp/.h (inside shapes)`: Example shape class. Copy these two files
    and replace/rename **every** instance of `Example` `example.h` or `EXAMPLE`
    with your new shape name.

* `triple.cpp/.h`: Triple class. Represents a 3-dimensional vector which is
    used for colors, points and vectors.
    Includes a number of useful functions and operators, see the comments in
    `triple.h`.
    Classes of `Color`, `Vector`, `Point` are all aliases of `Triple`.

* `objloader.cpp/.h`: Is a similar class to Model used in the OpenGL
    exercises to load .obj model files. It produces a std::vector
    of Vertex structs. See `vertex.h` on how you can retrieve the
    coordinates and other data defined at vertices.

### Supporting source files (Code directory)

* `lode/*`: Code for reading from and writing to PNG files,
    used by the `Image` class.
    lodepng is created by Lode Vandevenne and can be found on
    [github](https://github.com/lvandeve/lodepng).
* `json/*`: Code for parsing JSON documents.
    Created by Niels Lohmann and available under the MIT license on
    [github](https://github.com/nlohmann/json).
    **Recommended:** Especially take a look at their README for more
    info on how to work with json files.
