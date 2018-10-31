#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Environment.h"

using namespace std;

class FileManager {
    private:
        ifstream inputFile;
        ofstream outputFile;

    public:
        FileManager() {}

        Environment* readFile(string fileName) {
            string line_header;
            Environment* env = new Environment();

            inputFile.open(fileName, ios::in);
            inputFile >> line_header >> env->at.x >> env->at.y >> env->at.z;
            inputFile >> line_header >> env->from.x >> env->from.y >> env->from.z;
            inputFile >> line_header >> env->up.x >> env->up.y >> env->up.z;
            inputFile >> line_header >> env->fov;
            inputFile >> line_header >> env->light_position.x >> env->light_position.y >> env->light_position.z;
            inputFile >> line_header >> env->light_color.x >> env->light_color.y >> env->light_color.z;
            inputFile >> line_header >> env->ambient_light.x >> env->ambient_light.y >> env->ambient_light.z;
            inputFile >> line_header >> env->background.x >> env->background.y >> env->background.z;

            inputFile >> line_header;
            while(!inputFile.eof()) {
                Object* o;
                if (line_header == "Sphere") {
                    Position c = Position();
                    double rad = 0;
                    inputFile >> line_header >> c.x >> c.y >> c.z;
                    inputFile >> line_header >> rad;
                    o = new Sphere(Position(), Position(), 0, c, rad);
                }
                else if (line_header == "Triangle") {
                    Position v1, v2, v3;
                    inputFile >> v1.x >> v1.y >> v1.z;
                    inputFile >> v2.x >> v2.y >> v2.z;
                    inputFile >> v3.x >> v3.y >> v3.z;
                    o = new Triangle(Position(), Position(), 0, v1, v2, v3);;
                }
                inputFile >> line_header >> line_header;
                if (line_header == "Diffuse") {
                    inputFile >> o->diffuse.x >> o->diffuse.y >> o->diffuse.z;
                    inputFile >> line_header >> o->specular.x >> o->specular.y >> o->specular.z;
                    inputFile >> line_header >> o->phong;
                }
                else if (line_header == "Reflective") {
                    inputFile >> line_header >> line_header >> line_header;
                }
                env->env.push_back(o);
                inputFile >> line_header;
            }
            inputFile.close();
            return env;
        }

        void prepOutputFile(string fileName, int w, int h, int max_color) {
            outputFile.open(fileName, ios::out);
            outputFile << "P3" << endl;
            outputFile << "# " << fileName << endl;
            outputFile << w << ' ' << h << endl;
            outputFile << max_color << endl;
        }

        void addColor(Color color) {
            outputFile << color.x << ' ' << color.y << ' ' << color.z << endl;
        }
        
        void closeOutput() {
            outputFile.close();
        }
};