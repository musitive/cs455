#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace std;

template<typename T>
class Vec3 {
    public:
        T x, y, z;

        Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
        Vec3(T v) : x(v), y(v), z(v) {}
        Vec3(T u, T v, T w) : x(u), y(v), z(w) {}

        Vec3<T> operator + (const Vec3<T>& b) {return Vec3<T>(x + b.x, y + b.y, z + b.z);}
        Vec3<T> operator - (const Vec3<T>& b) {return Vec3<T>(x - b.x, y - b.y, z - b.z);}
        Vec3<T> operator * (const Vec3<T>& b) {return Vec3<T>(x * b.x, y * b.y, z * b.z);}
        Vec3<T> operator * (const float& b) {return Vec3<T>(x * b, y * b, z * b);}
        Vec3<T> operator / (const float& b) {return Vec3<T>(x / b, y / b, z / b);}
        Vec3<T> operator - () {return Vec3<T>(-x, -y, -z);}
};

typedef Vec3<float> Position;
typedef Vec3<int> Color;

template<typename T>
float length(Vec3<T> vec) {
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

Position normalize(const Position& vec) {
    float l = length(vec);
    if (l != 0)
        return Position(vec.x / l, vec.y / l, vec.z / l);
    else
        return vec;
}

template<typename T>
T dot(Vec3<T> vec1, Vec3<T> vec2) {
    Vec3<T> v = vec1 * vec2;
    return v.x + v.y + v.z;
}

template<typename T>
Color bound(const Vec3<T>& v, float min=0, float max=1) {

    return Color(clamp(v.x, min, max) * 255.0f, clamp(v.y, min, max) * 255.0f, clamp(v.z, min, max) * 255.0f);
}

class Ray {
    public:
        Position r0;
        Position rd;

        Ray(Position eye, Position s) : r0(eye), rd(s-eye) {}
};

class Sphere {
    public:
        Position c;
        float rad;
        Position diffuse;
        Position specular;
        float phong;

        Sphere() : c(Position(0, 0, -0.25)), rad(0.2), diffuse(Position(.7, 0, 0)), specular(Position(1)), phong(100) {}

        Color getColor(Position p, Position Light = Position(-.5, .5, -.5)) {
            Position np = normalize(p);
            Position n = normalize((np - c) / rad);
            Position l = normalize(Light);
            float dt = dot(n, l);

            Position r = n * dt * 2 - l;
            Position eye(0);

            Position light = Position(1);
            Position ambient = Position(1) * .2;
            Position d(0);
            Position ph(0);

            if (dt > 0) {
                d = light * max(0.0f, dt) * .5;
                ph = light * specular * pow(max(0.0f, dot(np, r)), phong) * .3;
            }

            return bound(diffuse * (ambient + d) + ph);
        }

        Color intersect(Ray r, int i) {
            i = 1;
            Ray o = r;
            r = Ray(normalize(r.r0), normalize(r.rd));
            float B = 2 * (r.rd.x * (r.r0.x - c.x) +
                           r.rd.y * (r.r0.y - c.y) +
                           r.rd.z * (r.r0.z - c.z));

            float C = (pow(r.r0.x, 2.0) - 2 * r.r0.x * c.x + pow(c.x, 2.0) +
                       pow(r.r0.y, 2.0) - 2 * r.r0.y * c.y + pow(c.y, 2.0) +
                       pow(r.r0.z, 2.0) - 2 * r.r0.z * c.z + pow(c.z, 2.0) -
                       pow(rad, 2.0));

            float D = pow(B, 2) - 4 * C;

            if (D < 0)
                return Color(i*2.99 + i*5.87 + i*1.14);

            float t1 = (-B + sqrt(D))/2.0;
            float t2 = (-B - sqrt(D))/2.0;
            float t = min(t1, t2);

            Position p = o.r0 + o.rd * t;

            return getColor(p);
        }
};

int main(int argc, char** argv) {
    const int WIDTH = atoi(argv[1]), HEIGHT = atoi(argv[2]);

    const int IMAX = WIDTH, IMIN = 0, JMAX = HEIGHT, JMIN = 0;
    const float UMAX = 1, UMIN = -1, VMAX = 1, VMIN = -1;
    const int MAX_COLOR = 255;

    ofstream ppm;
    ppm.open(argv[3], ios::out);
    ppm << "P3" << endl;
    ppm << "# " << argv[3] << endl;
    ppm << WIDTH << ' ' << HEIGHT << endl;
    ppm << MAX_COLOR << endl;

    // 1.	Send a ray from the eye through the screen
    // 2.	Determine which object that ray first intersects
    // 3.	Compute pixel color

    Position origin = Position();
    Position eye = origin;
    float d = -0.2;

    Sphere sphere = Sphere();

    for (int j = HEIGHT; j--;) {
        float v = (j - JMIN) * (VMAX - VMIN) / (JMAX - JMIN) + VMIN;
        for (int i = WIDTH; i--;) {
            float u = (i - IMIN) * (UMAX - UMIN) / (IMAX - IMIN) + UMIN;
            float w = d;
            Ray ray(eye, Position(u, v, w));

            Color color = sphere.intersect(ray, i);
            ppm << color.x << ' ' << color.y << ' ' << color.z << endl;
        }
    }

    return 0;
}