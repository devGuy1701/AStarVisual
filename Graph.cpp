//
// Created by Guido on 31/03/2020.
//

#include <list>
#include <cmath>
#include <algorithm>
#include <time.h>
#include <vector>

#include "Graph.h"

Graph::Graph(int w, int h) : width(w), height(h), window(sf::VideoMode(600, 600), "Mappa con risoluzione percorso A*") {

    srand( time(NULL) );

    map = new Cell[width * height];
    startCell = nullptr;
    endCell = nullptr;

    valid = sf::Color(128, 212, 255);

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            //implementazione iniziale randomica per celle visitabili e non
            map[y*width + x] = Cell(x, y, rand()%4 == 0 ? 9 : 2);
        }
    }
    //current = startCell;
}

void Graph::setStart(int x, int y) {
    map[y*width + x].setValue(1);
    startCell = &map[y*width + x];
    current = startCell;
}

void Graph::setEnd(int x, int y) {
    map[y*width + x].setValue(3);
    endCell = &map[y*width + x];
}

void Graph::setBlock(int x, int y) {
    map[y*width + x].setValue(9);
}

void Graph::printMap() {
    int cellSize = 60;

    Cell* oldcell = nullptr;

    setPattern(cellSize, pattern);

    //interfaccia grafica

    while(window.isOpen()) {

        sf::Event event;

        while(window.pollEvent(event)) {

            if(event.type == sf::Event::Closed)
                window.close();

            //al click del mouse
            if(event.type == sf::Event::MouseButtonPressed) {

                if(event.mouseButton.button == sf::Mouse::Left) {
                    setBUCell(cellSize, oldcell, event);
                } else if(event.mouseButton.button == sf::Mouse::Right) {
                    oldcell = setDestinationCell(cellSize, oldcell, event);
                } else {}

            }


        }


        window.clear(sf::Color::Black);

        //disegno le celle
        for(auto &it : pattern)
            window.draw(it);

        //disegno il tragitto
        drawPath(cellSize, window);

        window.display();
    }

}

void Graph::setPattern(int cellSize, std::vector<sf::RectangleShape> &pattern) const {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {

            int v = map[y*width + x].getValue();

            sf::RectangleShape rectangleShape;
            rectangleShape.setSize(sf::Vector2f(cellSize, cellSize));
            rectangleShape.setPosition(x*cellSize, y*cellSize);
            rectangleShape.setOutlineColor(sf::Color::White);
            rectangleShape.setOutlineThickness(2.0);

            //in base al valore assegno un colore
            if(v == 1) rectangleShape.setFillColor(sf::Color::Green); //inizio
            if(v == 2) rectangleShape.setFillColor(valid); //cella valida per movimento
            if(v == 3) rectangleShape.setFillColor(sf::Color::Red); //arrivo
            if(v == 9) rectangleShape.setFillColor(sf::Color::Black); //cella bloccata

            pattern.push_back(rectangleShape);

        }
    }
}

void Graph::setBUCell(int cellSize, const Cell *oldCell, const sf::Event &event) {
    int x = event.mouseButton.x;
    int y = event.mouseButton.y;

    for(auto &it : pattern) {

        int rx = int(it.getPosition().x);
        int ry = int(it.getPosition().y);
        int rs = int(it.getSize().x);

        if(x >= rx && x <= (rx + rs) && y >= ry && y <= (ry + rs)) { //blocco

            if(it.getFillColor() == sf::Color::Red)//non posso bloccare il blocco su cui mi trovo
                break;

            if(it.getFillColor() == sf::Color::Black) {//se bloccato lo rendo libero
                it.setFillColor(sf::Color(valid));
                map[(ry / cellSize) * width + (rx / cellSize)].setValue(2);
                break;
            }

            //se cella libera, allora blocco
            it.setFillColor(sf::Color::Black);
            map[(ry / cellSize) * width + (rx / cellSize)].setValue(9);

            if(oldCell != nullptr) {
                for(auto &g : pattern) {
                    if(g.getFillColor() == sf::Color::Green) {
                        g.setFillColor(valid);
                        break;
                    }
                }
                startPathFinding();
            }
            break;
        }
    }

}

Cell *Graph::setDestinationCell(int cellSize, Cell *oldCell, const sf::Event &event) {
    int x = event.mouseButton.x;
    int y = event.mouseButton.y;

    for(auto &it : pattern) {
        int rx = int(it.getPosition().x);
        int ry = int(it.getPosition().y);
        int rs = int(it.getSize().x);

        if(x >= rx && x <= (rx + rs) && y >= ry && y <= (ry + rs) ) { // cella su cui ho cliccato
            if(it.getFillColor() == sf::Color::Black || it.getFillColor() == sf::Color::Red) // non posso andare su un blocco o dove già sono
                break;

            if(endCell != nullptr && oldCell != nullptr) { // se ho fatto almeno una volta l'algoritmo
                //trovo la vecchia end e la metto valida(colore)
                setCellColor(valid, oldCell->getX(), oldCell->getY(), cellSize);
            }

            setEnd(rx / cellSize, ry / cellSize);

            //controllo che la destinazione sia raggiungibile
            startPathFinding();
            if(endCell->getParent() == nullptr)
                break;

            int ex = endCell->getX();
            int ey = endCell->getY();

            //metto verde la cella start
            setCellColor(sf::Color::Green, startCell->getX(), startCell->getY(), cellSize);

            it.setFillColor(sf::Color::Red);

            oldCell = startCell;
            startCell = endCell;

            break;
        }
    }
    return oldCell;
}

void Graph::setCellColor(sf::Color color, int px, int py, int cellSize) {
    for(auto &g : pattern) {
        int gx = g.getPosition().x;
        int gy = g.getPosition().y;

        if(gx == px*cellSize && gy == py*cellSize) {
            g.setFillColor(color);
            break;
        }
    }
}

float Graph::distance(Cell *a, Cell *b) {
    return sqrtf((a->getX() - b->getX()) * (a->getX() - b->getX()) +  (a->getY() - b->getY()) * (a->getY() - b->getY()));
}

void Graph::startPathFinding() {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            map[y * width + x].setVisited(false);
            map[y * width + x].setGlobalGoal(INFINITY);
            map[y * width + x].setLocalGoal(INFINITY);
            map[y * width + x].setParent(nullptr);
        }
    }

    current = startCell;
    startCell->setLocalGoal(0);
    startCell->setGlobalGoal(distance(startCell, endCell));

    std::list<Cell*> listNotTestedCells;
    listNotTestedCells.push_back(startCell);

    while(!listNotTestedCells.empty() && current != endCell) {
        listNotTestedCells.sort( [](const Cell* lhs, const Cell* rhs) { return lhs->getGlobalGoal() < rhs->getGlobalGoal(); } );

        while(!listNotTestedCells.empty() && listNotTestedCells.front()->isVisited())
            listNotTestedCells.pop_front();

        if(listNotTestedCells.empty())
            break;

        current = listNotTestedCells.front();
        current->setVisited(true);

        int x = current->getX();
        int y = current->getY();
        Cell *cellNeighbour = nullptr;

        if( y > 0) {
            cellNeighbour = &map[(y - 1) * width + (x + 0)];
            if(!cellNeighbour->isVisited() && cellNeighbour->getValue() != 9)
                listNotTestedCells.push_back(cellNeighbour);

            float possiblyLowerGoal = current->getLocalGoal() + distance(current, cellNeighbour);

            if(possiblyLowerGoal < cellNeighbour->getLocalGoal()) {
                cellNeighbour->setParent(current);
                cellNeighbour->setLocalGoal(possiblyLowerGoal);
                cellNeighbour->setGlobalGoal(cellNeighbour->getLocalGoal() + distance(cellNeighbour, endCell));
            }
        }
        if( y < height - 1) {
            cellNeighbour = &map[(y + 1) * width + (x + 0)];
            if(!cellNeighbour->isVisited() && cellNeighbour->getValue() != 9)
                listNotTestedCells.push_back(cellNeighbour);

            float possiblyLowerGoal = current->getLocalGoal() + distance(current, cellNeighbour);

            if(possiblyLowerGoal < cellNeighbour->getLocalGoal()) {
                cellNeighbour->setParent(current);
                cellNeighbour->setLocalGoal(possiblyLowerGoal);
                cellNeighbour->setGlobalGoal(cellNeighbour->getLocalGoal() + distance(cellNeighbour, endCell));
            }
        }
        if( x > 0) {
            cellNeighbour = &map[(y + 0) * width + (x - 1)];
            if(!cellNeighbour->isVisited() && cellNeighbour->getValue() != 9)
                listNotTestedCells.push_back(cellNeighbour);

            float possiblyLowerGoal = current->getLocalGoal() + distance(current, cellNeighbour);

            if(possiblyLowerGoal < cellNeighbour->getLocalGoal()) {
                cellNeighbour->setParent(current);
                cellNeighbour->setLocalGoal(possiblyLowerGoal);
                cellNeighbour->setGlobalGoal(cellNeighbour->getLocalGoal() + distance(cellNeighbour, endCell));
            }
        }
        if( y < width - 1) {
            cellNeighbour = &map[(y + 0) * width + (x + 1)];
            if(!cellNeighbour->isVisited() && cellNeighbour->getValue() != 9)
                listNotTestedCells.push_back(cellNeighbour);

            float possiblyLowerGoal = current->getLocalGoal() + distance(current, cellNeighbour);

            if(possiblyLowerGoal < cellNeighbour->getLocalGoal()) {
                cellNeighbour->setParent(current);
                cellNeighbour->setLocalGoal(possiblyLowerGoal);
                cellNeighbour->setGlobalGoal(cellNeighbour->getLocalGoal() + distance(cellNeighbour, endCell));
            }
        }

    }

}

void Graph::drawPath(int cellSize, sf::RenderWindow &window) {
    if(endCell != nullptr) {
        Cell *p = endCell;
        while(p->getParent() != nullptr) {
            sf::Vertex line[2];

            line[0].position = sf::Vector2f(p->getX()*cellSize + cellSize / 2, p->getY()*cellSize + cellSize / 2);
            line[0].color = sf::Color::Black;

            line[1].position = sf::Vector2f(p->getParent()->getX()*cellSize + cellSize / 2, p->getParent()->getY()*cellSize + cellSize / 2);
            line[1].color = sf::Color::Black;

            window.draw(line, 2, sf::Lines);
            p = p->getParent();
        }
    }
}