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
        int get_cols() { return cols; }
        Cell& get_cell(Coord target) { return buffer[target.get_y()][target.get_x()]; }
        void draw(Coord origin);
        void clear_lines();
        bool game_over();

    private:
        bool row_filled(int row);
        void clear_row(int row);
        void drop_rows(int row);
        int rows;
        int cols;
        std::vector<std::vector<Cell>> buffer;
};

#endif // PLAYFIELD_HPP
