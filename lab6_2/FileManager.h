#pragma once
#include <iostream>
#include <fstream>
#include "Vec.h"
#include "Environment.h"

using namespace std;

class FileManager {
    private:
        ifstream inputFile;
        ofstream outputFile;

    public:
        FileManager();

        Environment* readFile(const string& fileName);
        void prepOutputFile(const string& fileName, const int& w, const int& h, const int& max_color);
        void addColor(const Colori& color);
        void closeOutput();
};
