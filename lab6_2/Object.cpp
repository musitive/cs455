#include <iostream>
#include "Object.h"
#include "Vec.h"

using namespace std;

const Position MISS = Position(numeric_limits<double>::infinity());

Position Sphere::findIntersect(Ray r, bool culling) {
    Position l = c - r.r0;
    double tca = dot(l, r.rd);

    if (tca < 0)
        return MISS;

    double d2 = dot(l, l) - tca * tca;

    if (d2 > pow(rad, 2))
        return MISS;

    double thc = sqrt(pow(rad, 2) - d2);
    double t0 = tca - thc;
    double t1 = tca + thc;

    Position p = r.findPosition(t0);

    return p;
}

Direction Sphere::computeNormal(Position p) {
    return Direction(p - c);
}

Position Triangle::findIntersect(Ray r, bool culling) {
    double kEpsilon = .000000001;
    Position v1v2 = v2 - v1;
    Position v1v3 = v3 - v1;
    Position pvec = cross(r.rd, v1v3);
    float det = dot(v1v2, pvec);
    if (det < kEpsilon && culling) return MISS;
    float invDet = 1 / det;
    Position tvec = r.r0 - v1;
    double u = dot(tvec, pvec) * invDet;
    if (u < 0 || u > 1) return MISS;
 
    Position qvec = cross(tvec, v1v2); 
    double v = dot(r.rd, qvec) * invDet; 
    if (v < 0 || u + v > 1) return MISS; 
 
    double t = dot(v1v3, qvec) * invDet; 
 
    return r.findPosition(t); 

//     Direction n = computeNormal(Position());
//     double area2 = length(n);
 
//     double NdotRayDirection = dot(n, r.rd);
//     if (fabs(NdotRayDirection) < kEpsilon)
//         return MISS;
 
//     double d = dot(n, v1);
 
//     double t = (dot(n, r.r0) + d) / NdotRayDirection;
//     if (t < 0)
//         return MISS;
 
//     Position p = r.findPosition(t);
 
//     Position c, edge, inner;
 
//     edge = v2 - v1;
//     inner = p - v1;
//     c = cross(edge, inner);
//     if (dot(n, c) < 0)
//         return MISS;
 
//     edge = v3 - v2;
//     inner = p - v2;
//     c = cross(edge, inner);
//     if (dot(n, c) < 0)
//         return MISS;
 
//     edge = v1 - v3;
//     inner = p - v3;
//     c = cross(edge, inner);
//     if (dot(n, c) < 0)
//         return MISS;
 
//     return p;
}

Direction Triangle::computeNormal(Position p) {
    Direction a = v2 - v1;
    Direction b = v3 - v1;
    return cross(a, b);
}

Colori Object::computeColor(Position from, Position p, Light light, bool blocked, Colord ambient) {
    Direction n = computeNormal(p);

    return m->computeColor(from, p, n, light, blocked, ambient);
}