#include "coord.h"

Coord coord_constructor (int y, int x) {
    Coord new_coord;

    new_coord.y = y;
    new_coord.x = x;

    return new_coord;
}
