//
// Created by guido on 13/08/2019.
//

#include <cmath>
#include "Cell.h"

Cell::Cell(int _x, int _y, int _v, bool vis) {
    if(_x >= 0) x = _x;
    else x = 0;
    if(_y >= 0) y = _y;
    else y = 0;
    value = _v;
    visited = vis;
}