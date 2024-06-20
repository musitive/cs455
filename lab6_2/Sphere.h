#pragma once

#include "Object.h"
#include "Vec.h"

class Sphere : public Object {
    private:
        Position center;
        double radius;

    public:
        Sphere(Position center, double radius);

        Position calculateIntersection(Ray ray, bool culling=true) override;
        Direction computeNormal(Position position) override;
};