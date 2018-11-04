#include "Vec.h"

using namespace std;

struct Light {
    Colord color;
    Position position;
};

class Material {
    public:
        bool isReflective = false;
        virtual Colori computeColor(Position from, Position p, Direction n, Light light, bool blocked, Colord ambient) = 0;
};

class Diffuse : public Material {
    public:
        Colord diffuse;
        Colord specular;
        double phong;
    public:
        Diffuse(Colord diffuse, Colord specular, double phong) : diffuse(diffuse), specular(specular), phong(phong) {}
        Colori computeColor(Position from, Position p, Direction n, Light light, bool blocked, Colord ambient) override {
            Direction l = Direction(light.position - p);
            double dt = dot(l, n);

            Direction r = n * dt * 2 - l;

            Colord d(0);
            Colord ph(0);

            if (!blocked) {
                d = light.color * max(0.0, dt);
                Direction v = from - p;
                ph = light.color * specular * pow(max(0.0, dot(v, r)), phong);
            }

            return bound(diffuse * (ambient + d) + ph);
        }
};

class Reflective : public Material {
    public:
        Colord reflective;
    public:
        Reflective(Colord reflective) : reflective(reflective) { isReflective = true; }
        Colori computeColor(Position from, Position p, Direction n, Light light, bool blocked, Colord ambient) override {
            return bound(reflective);
        }
};