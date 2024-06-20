#pragma once

#include "FileManager.h"
#include "Object.h"

using namespace std;

FileManager::FileManager() {}

Environment* FileManager::readFile(const string& fileName) {
    string line_header;
    Environment* env = new Environment();

    input_file.open(fileName, ios::in);
    input_file >> line_header >> env->width >> line_header >> env->height;
    input_file >> line_header >> env->at.x >> env->at.y >> env->at.z;
    input_file >> line_header >> env->from.x >> env->from.y >> env->from.z;
    input_file >> line_header >> env->up.x >> env->up.y >> env->up.z;
    input_file >> line_header >> env->fov;
    input_file >> line_header >> env->light_position.x >> env->light_position.y >> env->light_position.z;
    input_file >> line_header >> env->light_color.x >> env->light_color.y >> env->light_color.z;
    input_file >> line_header >> env->ambient_light.x >> env->ambient_light.y >> env->ambient_light.z;
    input_file >> line_header >> env->background.x >> env->background.y >> env->background.z;

    input_file >> line_header;
    while(!input_file.eof()) {
        Object* o;
        if (line_header == "Sphere") {
            Position c = Position();
            double rad = 0;
            input_file >> line_header >> c.x >> c.y >> c.z;
            input_file >> line_header >> rad;
            o = new Sphere(c, rad);
        }
        else if (line_header == "Triangle") {
            Position v1, v2, v3;
            input_file >> v1.x >> v1.y >> v1.z;
            input_file >> v2.x >> v2.y >> v2.z;
            input_file >> v3.x >> v3.y >> v3.z;
            o = new Triangle(v1, v2, v3);;
        }
        input_file >> line_header >> line_header;
        Material* material;
        if (line_header == "Diffuse") {
            Colord diffuse = Colord();
            Colord specular = Colord();
            double phong = 0;
            input_file >> diffuse.x >> diffuse.y >> diffuse.z;
            input_file >> line_header >> specular.x >> specular.y >> specular.z;
            input_file >> line_header >> phong;
            material = new Diffuse(diffuse, specular, phong);
        }
        else if (line_header == "Reflective") {
            Colord reflective = Colord();
            input_file >> reflective.x >> reflective.y >> reflective.z;
            material = new Reflective(reflective);
        }
        o->setMaterial(material);
        env->env.push_back(o);
        input_file >> line_header;
    }
    input_file.close();
    return env;
}

void FileManager::prepOutputFile(const string& fileName, const int& w, const int& h, const int& max_color) {
    output_file.open(fileName, ios::out);
    output_file << "P3" << endl;
    output_file << "# " << fileName << endl;
    output_file << w << ' ' << h << endl;
    output_file << max_color << endl;
}

void FileManager::addColor(const Colori& color) {
    output_file << color << endl;
}

void FileManager::closeOutput() {
    output_file.close();
}
