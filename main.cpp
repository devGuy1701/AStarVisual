#include <iostream>
#include "Graph.h"

int main() {

    std::cout << "Mappa grafica con risoluzione algoritmo a*" << std::endl;

    int width = 10;
    int height = 10;

    Graph g (width, height);

    g.setStart(0,0);
    g.printMap();

    return 0;
}