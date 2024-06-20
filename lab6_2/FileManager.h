#pragma once
#include <iostream>
#include <fstream>
#include "Vec.h"
#include "Environment.h"
#include "Triangle.h"
#include "Sphere.h"

using namespace std;

class FileManager {
    private:
        ifstream input_file;
        ofstream output_file;



    public:
        FileManager();

        Environment* readFile(const string& filename);
        void prepOutputFile(const string& filename, const int& w, const int& h, const int& max_color);
        void addColor(const Colori& color);
        void closeOutput();
};
