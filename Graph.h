//
// Created by Guido on 31/03/2020.
//

#ifndef ASTARVISUAL_GRAPH_H
#define ASTARVISUAL_GRAPH_H


//librerie grafiche

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Image.hpp>


#include "Cell.h"

class Graph {
public:
    Graph(int width, int height);
    ~Graph() {
        delete map;
        delete startCell;
        delete endCell;
        delete  current;
    }

private:
    //Mappa formata da celle logiche
    Cell* map;
    int width, height;
    Cell* startCell;
    Cell* endCell;
    Cell* current;

    //Finestra per visualizzazione e mappa grafica
    sf::RenderWindow window;
    std::vector<sf::RectangleShape> pattern;

};


#endif //ASTARVISUAL_GRAPH_H
