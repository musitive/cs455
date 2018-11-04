#pragma once
#include <vector>

#include "Object.h"

using namespace std;

class Environment {
    public:
        double width, height;
        Position at;
        Position from;
        Position up;
        double fov = 0;
        Position light_position;
        Colord light_color;
        Colord ambient_light;
        Colord background;
        vector<Object*> env;

        Environment() {
            at = Position();
            from = Position();
            up = Position();
            fov = 0;
            light_position = Position();
            light_color = Position();
            ambient_light = Position();
            background = Position();
            env = vector<Object*>();
        }

};
