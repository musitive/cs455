#pragma once

#include <iostream>
#include "Material.h"
#include "Vec.h"

const double INF = numeric_limits<double>::infinity();
const Position MISS = Position(INF);

class Object {
    private:
        Material* material;

    public:
        void setMaterial(Material* material);
        Material* getMaterial();

        virtual Direction computeNormal(Position p) = 0;
        virtual Position findIntersect(Ray r, bool culling=true) = 0;
        virtual Colori computeColor(Position from, Position p, Light light, bool blocked, Colord ambient);
};

class Triangle : public Object {
    private:
        Position v1, v2, v3;
    public:
        Triangle(Position v1, Position v2, Position v3) : v1(v1), v2(v2), v3(v3) {}
        Position findIntersect(Ray r, bool culling=true) override;
        Direction computeNormal(Position p) override;
};

