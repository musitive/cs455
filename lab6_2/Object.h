#pragma once

#include <iostream>
#include "Material.h"
#include "Vec.h"

using namespace std;

class Object {
    private:
        Material* m;

    public:
        void setMaterial(Material* m) { this->m = m; }
        Material* getMaterial() { return m; }
        virtual Direction computeNormal(Position p) = 0;
        virtual Position findIntersect(Ray r, bool culling=true) = 0;
        virtual Colori computeColor(Position from, Position p, Light light, bool blocked, Colord ambient);
};

class Sphere : public Object {
    private:
        Position c;
        double rad;
    public:
        Sphere(Position c, double rad) : c(c), rad(rad) {}
        Position findIntersect(Ray r, bool culling=true) override;
        Direction computeNormal(Position p) override;
};

class Triangle : public Object {
    private:
        Position v1, v2, v3;
    public:
        Triangle(Position v1, Position v2, Position v3) : v1(v1), v2(v2), v3(v3) {}
        Position findIntersect(Ray r, bool culling=true) override;
        Direction computeNormal(Position p) override;
};

