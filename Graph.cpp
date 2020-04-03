//
// Created by Guido on 31/03/2020.
//

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
    current = startCell;


}

void Graph::setStart(int x, int y) {
    map[y*width + x].setValue(1);
    startCell = &map[y*width + x];
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
                    //algoritomo a*...
                } else {}

            }


        }


        window.clear(sf::Color::Black);


        //disegno le celle
        for(auto &it : pattern)
            window.draw(it);

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
            }
            break;
        }
    }

}