//
// Created by guido on 13/08/2019.
//

#ifndef ASTARVISUAL_CELL_H
#define ASTARVISUAL_CELL_H

#include <vector>

class Cell {
public:
    explicit Cell(int _x = 0, int _y = 0, int _v = 0, bool vis = false);

    int getX() const { return x; }
    int getY() const { return y; }
    bool isVisited() const { return visited; }
    int getValue() const { return value; }

    void setX(int _x) { x = _x >= 0 ? _x : 0; }
    void setY(int _y) { y = _y >= 0 ? _y : 0; }
    void setVisited(bool vis) { visited = vis; }
    void setValue(int val) { value = val; }

    bool operator==(const Cell& c) const {
        return (x == c.getX() && y == c.getY());
    }

    bool operator!=(const Cell& c) const {
        return (x != c.getX() && y != c.getY());
    }

private:
    int x, y, value;
    bool visited;
};


#endif //ASTARVISUAL_CELL_H
