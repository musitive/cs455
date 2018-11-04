#include "Vec.h"
#include "Object.h"
#include "Environment.h"
#include <numeric>
#include <random>

using namespace std;

class RayTracer {
    private:
        const int MAX_DEPTH = 4;
    public:
        double jitter(int i, double scale);
        Ray jitter(Ray r);
        Ray computeRay(double i, double j, double angle, double aspectratio, Environment* env);
        Colori trace(Ray ray, Environment* env, Object* current, const int& depth);
        vector<Colori> subdivide(int i, int j, double scale, double angle, double aspectratio, Environment* env);
};