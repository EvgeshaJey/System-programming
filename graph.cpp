#include "graph.h"
#include <cstdlib>
#include <string>  // Добавляем этот заголовочный файл

void plotGraph(const char* filename) {
    std::string command = "gnuplot -e \"set title 'График квадратного уравнения'; "
                          "set xlabel 'x'; set ylabel 'y'; "
                          "plot '" + std::string(filename) + "' using 1:2 with lines title 'y = f1(x)/f2(x)'\"";
    system(command.c_str());
}
