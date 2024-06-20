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
        virtual Position calculateIntersection(Ray ray, bool culling=true) = 0;
        virtual Colori computeColor(Position from, Position p, Light light, bool blocked, Colord ambient);
};
