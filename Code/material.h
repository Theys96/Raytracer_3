#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "triple.h"
#include "image.h"

class Material
{
    public:
        Color color;        // color
        Image* texture;      // templating
        bool usingTexture;

        double ka;          // ambient intensity
        double kd;          // diffuse intensity
        double ks;          // specular intensity
        double n;           // exponent for specular highlight size

        Material() = default;

        Material(Color const &color, double ka, double kd, double ks, double n)
        :
            color(color),
            usingTexture(false),
            ka(ka),
            kd(kd),
            ks(ks),
            n(n)
        {}

        Material(std::string const &texture, double ka, double kd, double ks, double n)
        :
            texture(new Image(texture)),
            usingTexture(true),
            ka(ka),
            kd(kd),
            ks(ks),
            n(n)
        {}
};

#endif
