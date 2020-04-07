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
#include <memory>
#include <cmath>

class Graph {
public:
    Graph(int width, int height);
    ~Graph() {
        delete[] map;
        delete startCell;
        delete endCell;
        delete current;
    }


    void setStart(int x, int y);
    void setEnd(int x, int y);
    void setBlock(int x, int y);

    void printMap();

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
    sf::Color valid;


    void setCellColor(sf::Color color, int px, int py, int cellSize);
    void setPattern(int cellSize, std::vector<sf::RectangleShape> &pattern) const;
    void setBUCell(int cellSize, const Cell *oldCell, const sf::Event &event);
    float distance(Cell* a, Cell* b);

    Cell *setDestinationCell(int cellSize, Cell *oldCell, const sf::Event &event);
    void startPathFinding();
    void drawPath(int cellSize, sf::RenderWindow &window);
};


#endif //ASTARVISUAL_GRAPH_H
