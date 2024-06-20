#pragma once

#include "Object.h"
#include "Vec.h"

class Triangle : public Object {
    private:
        Position v1, v2, v3;
    public:
        Triangle(Position v1, Position v2, Position v3) : v1(v1), v2(v2), v3(v3) {}
        Position calculateIntersection(Ray ray, bool culling=true) override;
        Direction computeNormal(Position p) override;
};

