#include "Sphere.h"
#include "Vec.h"

Sphere::Sphere(Position c, double rad) : c(c), radius(rad) {}

Position Sphere::findIntersect(Ray r, bool culling) {
    Position l = c - r.r0;
    double tca = dot(l, r.rd);

    if (tca < 0)
        return MISS;

    double d2 = dot(l, l) - tca * tca;

    if (d2 > pow(radius, 2))
        return MISS;

    double thc = sqrt(pow(radius, 2) - d2);
    double t0 = tca - thc;
    double t1 = tca + thc;

    Position p = r.findPosition(t0);

    return p;
}

Direction Sphere::computeNormal(Position p) {
    return Direction(p - c);
}