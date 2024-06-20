#include <iostream>
#include <numeric>
#include <random>
// #include <omp.h>
#include "FileManager.h"
#include "Object.h"
#include "RayTracer.h"

using namespace std;

const double UMAX = 1, UMIN = -1, VMAX = -1, VMIN = 1;
const int MAX_COLOR = 255;

template<typename T>
Vec3<T> average(vector<Vec3<T>> v) {
    Vec3<T> t = Vec3<T>();
    for(Vec3<T> c: v) {
        t = t + c;
    }
    return t / v.size();
};

int main(int argc, char** argv) {
    char* input_filename = argv[1];
    char* output_filename = argv[2];

    srand(time(0));

    FileManager* fm = new FileManager();
    Environment* env = fm->readFile(input_filename);

    const int IMAX = env->width, IMIN = 0, JMAX = env->height, JMIN = 0;

    fm->prepOutputFile(output_filename, IMAX, JMAX, MAX_COLOR);
    double fov = env->fov, aspectratio = IMAX / double(JMAX);
    double angle = tan(M_PI * 0.5 * fov / 180.);

    RayTracer* rt = new RayTracer();

    for(int j = 0; j < JMAX; j++) {
        for(int i = 0; i < IMAX; i++) {
            vector<Colori> colors = rt->subdivide(i, j, 4, angle, aspectratio, env);
            fm->addColor(average(colors));
        }
    }
    
    fm->closeOutput();

    return 0;
}