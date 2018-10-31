#include "FileManager.h"

using namespace std;

class Shader {

};

class RayTracer {

};

int main(int argc, char** argv) {
    const int WIDTH = atoi(argv[1]), HEIGHT = atoi(argv[2]);

    const int IMAX = WIDTH, IMIN = 0, JMAX = HEIGHT, JMIN = 0;
    const int MAX_COLOR = 255;

    FileManager* fm = new FileManager();
    Environment* env = fm->readFile(argv[3]);

    fm->prepOutputFile(argv[4], WIDTH, HEIGHT, MAX_COLOR);

    Position e1 = normalize(env->from - env->at);
    Position e2 = normalize(cross(env->up, e1));
    Position e3 = normalize(cross(e1, e2));

    double d = length(env->from - env->at) / tanf(env->fov * M_PI / 360.);
    double UMAX = d, UMIN = -d, VMAX = d, VMIN = -d;

    double prev = 0;

    for (int j = 0; j < HEIGHT; j++) {
        double v = (j - JMIN) * (VMAX - VMIN) / (JMAX - JMIN) + VMIN;
        // double NDCy = (j + .5) / (double) HEIGHT;
        // double v = (1 - 2 * NDCy) * tanf(env->fov * M_PI / 360.) * length(env->from - env->at) ;
        for (int i = 0; i < WIDTH; i++) {
            double u = (i  - IMIN) * (UMAX - UMIN) / (IMAX - IMIN) + UMIN;
            // double NDCx = (i + .5) / (double) WIDTH;
            // double u = (2 * NDCx - 1) * (WIDTH / (double) HEIGHT) * tanf(env->fov * M_PI / 360.) * length(env->from - env->at);
            double w = 0;
            Ray ray(env->from, Position(u, v, w) + env->at);
            Object* closest = NULL;
            Position* p = new Position(-numeric_limits<double>::infinity());
            Position np;
            Color color;

            for(Object* o: env->env) {
                np = o->intersect(ray);
                if (np.z > p->z) {
                    closest = o;
                    *p = np;
                }
            }

            if (closest) {
                color = closest->getColor(*p);
            }
            else {
                color = Color(env->background.x * 255);
            }
            fm->addColor(color);
        }
    }
    fm->closeOutput();

    return 0;
}