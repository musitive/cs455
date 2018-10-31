#include "Vec.h"
#include "Object.h"
#include "Environment.h"
#include <numeric>
#include <random>

#define PLACE 100
const Colori BACKGROUND = Colori(50);

using namespace std;

class RayTracer {
    public:
        double jitter(int i, double scale);
        Ray jitter(Ray r);
        Ray computeRay(double i, double j, double angle, double aspectratio, Environment* env);
        Colori trace(Ray ray, Environment* env);
        vector<Colori> subdivide(int i, int j, double scale, double angle, double aspectratio, Environment* env);
};