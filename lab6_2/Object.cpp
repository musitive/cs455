#include <iostream>
#include "Object.h"
#include "Vec.h"

using namespace std;

void Object::setMaterial(Material* material) {
    this->material = material;
}

Material* Object::getMaterial() {
    return this->material;
}

Colori Object::computeColor(Position from, Position p, Light light, bool blocked, Colord ambient) {
    Direction n = computeNormal(p);

    return material->computeColor(from, p, n, light, blocked, ambient);
}