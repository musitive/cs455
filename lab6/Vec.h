#pragma once

#include <iostream>
#include <cmath>
#include <algorithm>

using namespace std;

template<typename T>
class Vec2 {
    public:
        T x, y;

        Vec2() : x(T(0)), y(T(0)) {}
        Vec2(T v) : x(v), y(v) {}
        Vec2(T u, T v, T w) : x(u), y(v) {}

        Vec2<T> operator + (const Vec2<T>& b) {return Vec2<T>(x + b.x, y + b.y);}
        Vec2<T> operator - (const Vec2<T>& b) {return Vec2<T>(x - b.x, y - b.y);}
        Vec2<T> operator * (const Vec2<T>& b) {return Vec2<T>(x * b.x, y * b.y);}
        Vec2<T> operator * (const double& b) {return Vec2<T>(x * b, y * b);}
        Vec2<T> operator / (const double& b) {return Vec2<T>(x / b, y / b);}
        Vec2<T> operator - () {return Vec2<T>(-x, -y);}
};

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
        Vec3<T> operator * (const double& b) {return Vec3<T>(x * b, y * b, z * b);}
        Vec3<T> operator / (const double& b) {return Vec3<T>(x / b, y / b, z / b);}
        Vec3<T> operator - () {return Vec3<T>(-x, -y, -z);}
};

typedef Vec3<double> Position;
typedef Vec3<int> Color;

class Ray {
    public:
        Position r0;
        Position rd;

        Ray(Position eye, Position s) : r0(eye), rd(s) {}
};

template<typename T>
double length(Vec3<T> vec) {
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}

Position normalize(const Position& vec) {
    double l = length(vec);
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
Vec3<T> cross(Vec3<T> vec1, Vec3<T> vec2) {
    return Vec3<T>(vec1.y * vec2.z - vec1.z * vec2.y,
                   vec1.z * vec2.x - vec1.x * vec2.z,
                   vec1.x * vec2.y - vec1.y * vec2.x);
}

Color bound(const Vec3<double>& v, double min=0, double max=1) {
    return Color(clamp(v.x, min, max) * 255.0, clamp(v.y, min, max) * 255.0, clamp(v.z, min, max) * 255.0);
}

template<typename T>
int inbounds(const Vec3<T> v, const Vec3<T> v1, const Vec3<T> v2, const int& c) {
    if (c == 0) {
        if ((v.y < v1.y && v.y > v2.y) || (v.y > v1.y && v.y < v2.y)) {
            T m = (v1.z - v2.z) / (v1.y - v2.y);
            T z = v1.z + m * (v.y - v1.y);
            return z > v.z;
        }
        else return false;
    }
    else if (c == 1) {
        if ((v.x < v1.x && v.x > v2.x) || (v.x > v1.x && v.x < v2.x)) {
            T m = (v1.z - v2.z) / (v1.x - v2.x);
            T z = v1.z + m * (v.x - v1.x);
            return z > v.z;
        }
        else return false;
    }
    else {
        // if (((v1.y <= v.y) && (v2.y > v.y)) || ((v1.y > v.y) && (v2.y <=  v.y))) {
        //     double vt = (double) (v.y - v1.y) / (v2.y - v1.y);
        //     if (v.x < v1.x + vt * (v2.x - v1.x))
        //         return true;
        // }

        if ((v.y < v1.y && v.y > v2.y) || (v.y > v1.y && v.y < v2.y)) {
            T m = (v1.x - v2.x) / (v1.y - v2.y);
            T x = m * (v.y - v1.y) + v1.x;
            return x > v.x;
        }

        else return false;
    }
}