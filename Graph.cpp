//
// Created by Guido on 31/03/2020.
//

#include <list>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <vector>
#include <iostream>

#include "Graph.h"

Graph::Graph(int w, int h) : width(w), height(h), window(sf::VideoMode(600, 600), "Mappa con risoluzione percorso A*") {

    srand( time(NULL) );

    map = new Cell[width * height];

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
    startCell = map[y*width + x];
}

void Graph::setEnd(int x, int y) {
    map[y*width + x].setValue(3);
    endCell = map[y*width + x];
}

void Graph::setBlock(int x, int y) {
    map[y*width + x].setValue(9);
}

int Graph::getIndex(const Cell &c) const {
    return (c.getY()*width + c.getX());
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

            if(map[getIndex(endCell)].isVisited() && oldCell != nullptr) { // se ho fatto almeno una volta l'algoritmo
                //trovo la vecchia end e la metto valida(colore)
                for(auto &g : pattern) {
                    if(g.getFillColor() == sf::Color::Green) {
                        g.setFillColor(valid);
                        break;
                    }
                }
                //setCellColor(valid, oldCell->getX(), oldCell->getY(), cellSize);
            }

            setEnd(rx / cellSize, ry / cellSize);

            //controllo che la destinazione sia raggiungibile
            startPathFinding();
            if(!map[getIndex(endCell)].isVisited())
                break;

            //metto verde la cella start
            setCellColor(sf::Color::Green, startCell.getX(), startCell.getY(), cellSize);

            it.setFillColor(sf::Color::Red);

            oldCell = &startCell;
            endCell.setVisited(false);
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

float Graph::distance(const Cell& a, const Cell& b) const {
    return sqrtf((a.getX() - b.getX()) * (a.getX() - b.getX()) +  (a.getY() - b.getY()) * (a.getY() - b.getY()));
}

void Graph::startPathFinding() {
    parents.clear();
    localGoals.clear();
    globalGoals.clear();

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            map[y * width + x].setVisited(false);
            localGoals.insert(std::pair<int,int>(y*width+x, 5000));
            globalGoals.insert(std::pair<int,int>(y*width+x, 5000));
            parents.insert(std::pair<int,int>(y*width+x, -1));//per ora non lo ha
        }
    }

    Cell current = startCell;
    localGoals.at(getIndex(current)) = 0;
    globalGoals.at(getIndex(current)) = distance(current, endCell);

    std::list<Cell> listNotTestedCells;
    listNotTestedCells.push_back(startCell);

    while(!listNotTestedCells.empty()) {

        if(current == endCell) {
            endCell.setVisited(true);
            map[getIndex(endCell)].setVisited(true);
            break;
        }

        listNotTestedCells.sort( [&](const Cell& lhs, const Cell& rhs) {
            return globalGoals.at(getIndex(lhs)) < globalGoals.at(getIndex(rhs));
        } );

        while(!listNotTestedCells.empty() && listNotTestedCells.front().isVisited())
            listNotTestedCells.pop_front();

        if(listNotTestedCells.empty())
            break;

        current = listNotTestedCells.front();
        listNotTestedCells.front().setVisited(true);
        current.setVisited(true);
        map[getIndex(current)].setVisited(true);

        int x = current.getX();
        int y = current.getY();

        if( y > 0) {
            addNeighbour(map[(y - 1) * width + (x + 0)], current, listNotTestedCells);
        }
        if( y < height - 1) {
            addNeighbour(map[(y + 1) * width + (x + 0)], current, listNotTestedCells);
        }
        if( x > 0) {
            addNeighbour(map[(y + 0) * width + (x - 1)], current, listNotTestedCells);
        }
        if( x < width - 1) {
            addNeighbour(map[(y + 0) * width + (x + 1)], current, listNotTestedCells);
        }

    }

}

void Graph::addNeighbour(const Cell &neighbour, const Cell &current, std::list<Cell> &listNotTestedCells) {
    if(!neighbour.isVisited() && neighbour.getValue() != 9)
        listNotTestedCells.push_back(neighbour);
    float possiblyLowerGoal = localGoals.at(getIndex(current)) + distance(current, neighbour);
    if(possiblyLowerGoal < localGoals.at(getIndex(neighbour))) {
        parents.at(getIndex(neighbour)) = getIndex(current);
        localGoals.at(getIndex(neighbour)) = possiblyLowerGoal;
        globalGoals.at(getIndex(neighbour)) = localGoals.at(getIndex(neighbour)) + distance(neighbour, endCell);
    }
}

void Graph::drawPath(int cellSize, sf::RenderWindow &window) const {

    std::map<int, int> temp;
    std::vector<int> path;

    for(auto p : parents)
        if(p.second != -1)
            temp.insert(std::pair<int, int>(p.first, p.second));

    int ex = endCell.getX();
    int ey = endCell.getY();

    for(auto p : temp) {
        int tx = p.first % 10;
        int ty = p.first / 10;

        if(ex == tx && ey == ty) {
            path.insert(path.begin(), p.second);
            path.insert(path.begin(), p.first);
            int h = p.second;
            while(true) {
                if(temp.find(h) != temp.end()) {
                    path.push_back(temp.at(h));
                    h = temp.at(h);
                } else break;
            }
            break;
        }
    }

    int k = 0;

    for(auto it = path.begin(); it != path.end(); ++it) {
        int ix = (*it) % 10;
        int iy = (*it) / 10;
        if(k < path.size() - 1) {
            int iix = (*(it+1)) % 10;
            int iiy = (*(it+1)) / 10;

            sf::Vertex line[2];
            line[0].position = sf::Vector2f(ix*cellSize + cellSize / 2, iy*cellSize + cellSize / 2);
            line[0].color = sf::Color::Black;

            line[1].position = sf::Vector2f(iix*cellSize + cellSize / 2, iiy*cellSize + cellSize / 2);
            line[1].color = sf::Color::Black;

            window.draw(line, 2, sf::Lines);
        } else break;
        k++;
    }


}
void Graph::setNoBlock() {
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            if(map[j * width + i].getValue() == 9)
                map[j * width + i].setValue(2);
        }
    }
}

Cell Graph::getCellFromIndex(int x, int y) const {
    return map[y*width + x];
}

int Graph::getPathSize() const {
    std::map<int, int> temp;
    std::vector<int> path;

    for(auto p : parents)
        if(p.second != -1)
            temp.insert(std::pair<int, int>(p.first, p.second));

    int ex = endCell.getX();
    int ey = endCell.getY();

    for(auto p : temp) {

        if(ex*width+ey == p.first) {
            path.insert(path.begin(), p.second);
            path.insert(path.begin(), p.first);
            int h = p.second;
            while(true) {
                if(temp.find(h) != temp.end()) {
                    path.push_back(temp.at(h));
                    h = temp.at(h);
                } else break;
            }
            break;
        }
    }
    return path.size();
}