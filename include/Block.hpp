#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "Coord.hpp"

class Block
{
    public:
        Block(Coord coord, int type);

        Coord get_coord() { return coord; }
        void set_coord(Coord val) { coord = val; }
        int get_type() { return type; }
        bool get_filled() { return filled; }
        void set_filled(bool val) { filled = val; }

    private:
        Coord coord;
        int type;
        bool filled;
};

#endif // BLOCK_HPP
