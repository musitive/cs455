#include <iostream>
#include <numeric>
#include <random>
#include "FileManager.h"
#include "Object.h"
#include "RayTracer.h"

using namespace std;

template<typename T>
Vec3<T> average(vector<Vec3<T>> v) {
    Vec3<T> t = Vec3<T>();
    for(Vec3<T> c: v) {
        t = t + c;
    }
    return t / v.size();
};

int main(int argc, char** argv) {
    srand(time(0));

    const int IMAX = PLACE, IMIN = 0, JMAX = PLACE, JMIN = 0;
    const double UMAX = 1, UMIN = -1, VMAX = -1, VMIN = 1;
    const int MAX_COLOR = 255;

    FileManager* fm = new FileManager();
    Environment* env = fm->readFile(argv[1]);
    fm->prepOutputFile(argv[2], IMAX, JMAX, MAX_COLOR);
    double fov = env->fov, aspectratio = PLACE / double(PLACE);
    double angle = tan(M_PI * 0.5 * fov / 180.);

    RayTracer* rt = new RayTracer();

    for(int j = 0; j < PLACE; j++) {
        for(int i = 0; i < PLACE; i++) {
            vector<Colori> colors = rt->subdivide(i, j, pow(2,2), angle, aspectratio, env);
            fm->addColor(average(colors));
        }
    }
    fm->closeOutput();

    return 0;
}