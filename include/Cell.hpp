#ifndef CELL_HPP
#define CELL_HPP

#include "Coord.hpp"

 enum { BLACK_BG_WHITE_FG, WHITE_ASCII, RED_ASCII, GREEN_ASCII, YELLOW_ASCII, BLUE_ASCII, CYAN_ASCII, MAGENTA_ASCII }; // ColorPairsASCII

 enum { SOLID_WHITE=1, SOLID_RED, SOLID_GREEN, SOLID_YELLOW, SOLID_BLUE, SOLID_CYAN, SOLID_MAGENTA }; // ColorPairsSolid

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
