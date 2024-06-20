#include "Sphere.h"
#include "Vec.h"

Sphere::Sphere(Position center, double radius) : center(center), radius(radius) {}

Position Sphere::calculateIntersection(Ray ray, bool culling) {
    // Distance between the center of the sphere and the origin of the ray
    // OC = S_c - r_0
    Position distance_to_origin = this->center - ray.origin;

    // tca = OC . r_d
    double tca = dot(distance_to_origin, ray.direction);

    if (tca < 0)
        return MISS;

    // d^2 = ||OC||^2 - tca^2
    double d_squared = dot(distance_to_origin, distance_to_origin) - pow(tca, 2);
    double radius_squared = pow(radius, 2);

    // if d^2 > r^2, the ray misses the sphere
    if (d_squared > radius_squared)
        return MISS;

    // thc = sqrt(r^2 - d^2)
    double thc = sqrt(radius_squared - d_squared);

    // if the origin of the ray is outside of the sphere
    double root = tca - thc;

    // else use the positive root
    // root = tca + thc;

    Position intersection = ray.findPositionOnRay(root);

    return intersection;
}

Direction Sphere::computeNormal(Position position) {
    return Direction(position - center);
}