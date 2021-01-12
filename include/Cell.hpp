#ifndef CELL_HPP
#define CELL_HPP

#include "Coord.hpp"

 enum { BLACK_BG_BLACK_FG, BLACK_BG_WHITE_FG, BLACK_BG_RED_FG, BLACK_BG_GREEN_FG, BLACK_BG_YELLOW_FG, BLACK_BG_BLUE_FG, BLACK_BG_CYAN_FG, BLACK_BG_MAGENTA_FG }; //ColorPairs;

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
