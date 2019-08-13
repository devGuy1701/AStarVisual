//
// Created by guido on 13/08/2019.
//

#ifndef ASTARVISUAL_CELL_H
#define ASTARVISUAL_CELL_H

#include <vector>

class Cell {
public:
    explicit Cell(int _x = 0, int _y = 0, int _v = 0, Cell* p = nullptr, bool vis = false);

    int getX() const { return x; }
    int getY() const { return y; }
    bool isVisited() const { return visited; }
    int getValue() const { return value; }
    float getGlobalGoal() const { return globalGoal; }
    float getLocalGoal() const { return localGoal; }
    Cell *getParent() const { return  parent; }
    std::vector<Cell*> getNeighbours() const { return vecNeighbours; }

    void setX(int _x) { x = _x >= 0 ? _x : 0; }
    void setY(int _y) { y = _y >= 0 ? _y : 0; }
    void setVisited(bool vis) { visited = vis; }
    void setValue(int val) { value = val; } //controllo valori?
    void setGlobalGoal(float global) { globalGoal = global; }
    void setLocalGoal(float local) { localGoal = local; }
    void setParent(Cell* p) { parent = p; }
    void addNeighbour(Cell* n) { vecNeighbours.push_back(n); }


private:
    int x, y, value;
    bool visited;
    float globalGoal, localGoal;
    std::vector<Cell*> vecNeighbours;
    Cell* parent;
};


#endif //ASTARVISUAL_CELL_H
