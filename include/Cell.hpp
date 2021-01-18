#ifndef CELL_HPP
#define CELL_HPP

#include "Coord.hpp"

class Cell
{
    public:
        Cell(Coord coord);

        int get_color() { return color; }
        void set_color(int val) { color = val; }
        bool get_filled() { return filled; }
        void set_filled(bool val) { filled = val; }
        Coord get_coord() { return coord; }

    private:
        int color;
        bool filled;
        Coord coord;
};

#endif // CELL_HPP
