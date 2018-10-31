#pragma once
#include "Vec.h"

using namespace std;

class Object {
    public:
        Position diffuse;
        Position specular;
        double phong;
        Position n;

    public:
        Object(Position d, Position s, double p) : diffuse(d), specular(s), phong(p) {};

        virtual Position intersect(Ray r) = 0;
        virtual Color getColor(Position p, Position Light = Position(-.5, .5, .5)) {
            Position np = normalize(p);
            n = normalize(n);
            Position l = normalize(Light);
            double dt = dot(n, l);

            Position r = n * dt * 2 - l;

            Position light = Position(1);
            Position ambient = Position(1) * .2;
            Position d(0);
            Position ph(0);

            if (dt > 0) {
                d = light * max(0.0, dt) * .5;
                ph = light * specular * pow(max(0.0, dot(np, r)), phong) * .3;
            }

            return bound(diffuse * (ambient + d) + ph);
        };
};


class Triangle : public Object {
    protected:
        Position v1, v2, v3;

    public:
        Triangle(Position d, Position s, double p, Position v1, Position v2, Position v3) : Object(d, s, p), v1(v1), v2(v2), v3(v3) {};

        Position intersect(Ray r) override {
            n = normalize(cross(v3-v1,v2-v1));
            double denom = dot(n, normalize(r.rd));
            
            if (denom == 0)
                return -numeric_limits<double>::infinity();

            double t = dot(n, normalize(r.r0-v1)) / denom;
            Position p = r.r0 + r.rd * t;

            int count = 0;
            int axis = abs(n.x) > abs(n.y) ? 0 : abs(n.y) > abs(n.z) ? 1 : 2;

            count += inbounds(p-p, v1-p, v2-p, axis);
            count += inbounds(p-p, v2-p, v3-p, axis);
            count += inbounds(p-p, v1-p, v3-p, axis);

            if (count % 2 == 1) {
                return p;
            }
            return -numeric_limits<double>::infinity();
        };
};

class Sphere : public Object {
    protected:
        Position c;
        double rad;

    public:
        Sphere(Position d, Position s, double p, Position c, double r) : Object(d, s, p), c(c), rad(r) {};

        Position intersect(Ray r) override {
            Ray o = r;
            r = Ray(normalize(r.r0), normalize(r.rd));
            double B = 2 * (r.rd.x * (r.r0.x - c.x) +
                            r.rd.y * (r.r0.y - c.y) +
                            r.rd.z * (r.r0.z - c.z));

            double C = (pow(r.r0.x, 2.0) - 2 * r.r0.x * c.x + pow(c.x, 2.0) +
                        pow(r.r0.y, 2.0) - 2 * r.r0.y * c.y + pow(c.y, 2.0) +
                        pow(r.r0.z, 2.0) - 2 * r.r0.z * c.z + pow(c.z, 2.0) -
                        pow(rad, 2.0));

            double D = pow(B, 2) - 4 * C;

            if (D < 0)
                return -numeric_limits<double>::infinity();

            double t1 = (-B + sqrt(D))/2.0;
            double t2 = (-B - sqrt(D))/2.0;
            double t = min(t1, t2);

            Position p = o.r0 + o.rd * t;
            Position np = normalize(p);
            n = (np - c) / rad;
            return p;
        };
};
