#include "Object.h"
#include "Vec.h"

class Sphere : public Object {
    private:
        Position c;
        double radius;
    public:
        Sphere(Position c, double rad) : c(c), radius(rad) {}
        Position findIntersect(Ray r, bool culling=true) override;
        Direction computeNormal(Position p) override;
};