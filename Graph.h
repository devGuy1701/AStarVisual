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
#include <map>
#include <list>

class Graph {
public:
    Graph(int width, int height);
    ~Graph() {
        delete[] map;
    }

    void setStart(int x, int y);
    void setEnd(int x, int y);
    void setBlock(int x, int y);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getIndex(const Cell& c) const;

    void printMap();

    void startPathFinding();


    void setNoBlock();
    Cell getCellFromIndex(int x, int y) const;
    int getPathSize() const;

private:
    //Mappa formata da celle logiche
    Cell* map;
    int width, height;

    Cell startCell;
    Cell endCell;

    //la chiave è l'indice (y*width+x) della cella nella mappa
    std::map<int, int> parents;
    std::map<int, int> localGoals;
    std::map<int, int> globalGoals;

    //Finestra per visualizzazione e mappa grafica
    sf::RenderWindow window;
    std::vector<sf::RectangleShape> pattern;
    sf::Color valid;


    void setCellColor(sf::Color color, int px, int py, int cellSize);
    void setPattern(int cellSize, std::vector<sf::RectangleShape> &pattern) const;
    void setBUCell(int cellSize, const Cell *oldCell, const sf::Event &event);
    float distance(const Cell& a, const Cell& b) const;

    Cell *setDestinationCell(int cellSize, Cell *oldCell, const sf::Event &event);
    void drawPath(int cellSize, sf::RenderWindow &window) const;
    void addNeighbour(const Cell &neighbour, const Cell &current, std::list<Cell>& listNotTestedCells);
};


#endif //ASTARVISUAL_GRAPH_H
