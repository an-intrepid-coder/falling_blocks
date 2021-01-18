#ifndef PLAYFIELD_HPP
#define PLAYFIELD_HPP

#define PLAYFIELD_HEIGHT 24
#define PLAYFIELD_WIDTH 10

#include <vector>
#include "Coord.hpp"
#include "Cell.hpp"

class Playfield
{
    public:
        Playfield(int rows, int cols);

        int get_rows() { return rows; }
        void set_rows(int val) { rows = val; }
        int get_cols() { return cols; }
        void set_cols(int val) { cols = val; }
        std::vector<int> clear_lines();
        Cell& get_cell(Coord target) { return buffer[target.get_y()][target.get_x()]; }
        void draw(Coord origin, bool grid);
        bool game_over();
        Coord get_origin() { return origin; }
        void set_origin(Coord val) { origin = val; }

    private:
        bool row_filled(int row);
        void clear_row(int row);
        void drop_rows(int row);
        int rows;
        int cols;
        std::vector<std::vector<Cell>> buffer;
        Coord origin = Coord(0, 0);
};

#endif // PLAYFIELD_HPP
