#include "file_io.h"
#include <fstream>

void writeDataToFile(const char* filename, double* xValues, double* yValues, int count) {
    std::ofstream outFile(filename);
    for (int i = 0; i < count; ++i) {
        outFile << xValues[i] << " " << yValues[i] << "\n";
    }
    outFile.close();
}
