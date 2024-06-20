#include "Triangle.h"

const double K_EPSILON = .000000001;

Position Triangle::calculateIntersection(Ray ray, bool culling) {
    Position edge_a = v2 - v1;
    Position edge_b = v3 - v1;

    Position pvec = cross_product(ray.direction, edge_b);

    float det = dot(edge_a, pvec);

    if (culling && det < K_EPSILON) return MISS;

    float invDet = 1 / det;

    Position tvec = ray.origin - v1;

    double u = dot(tvec, pvec) * invDet;

    if (u < 0 || u > 1) return MISS;
 
    Position qvec = cross_product(tvec, edge_a); 

    double v = dot(ray.direction, qvec) * invDet; 

    if (v < 0 || u + v > 1) return MISS; 
 
    double t = dot(edge_b, qvec) * invDet; 
 
    return ray.findPositionOnRay(t); 
}

Direction Triangle::computeNormal(Position p) {
    Direction a = v2 - v1;
    Direction b = v3 - v1;
    return cross_product(a, b);
}