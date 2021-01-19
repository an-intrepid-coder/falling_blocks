#include <ncurses.h>
#include "Playfield.hpp"
#include "Tetromino.hpp"

using std::vector;

Playfield::Playfield(int rows, int cols) : rows(rows), cols(cols)
{
    for (auto row = 0; row < rows; row++)
    {
        vector<Cell> new_row;
        for (auto col = 0; col < cols; col++)
            new_row.push_back(Cell(Coord(row, col)));

        buffer.push_back(new_row);
    }
}

bool Playfield::row_filled(int row)
{
    for (auto cell : buffer[row])
    {
        if (!cell.get_filled())
            return false;
    }
    return true;
}

void Playfield::clear_row(int row)
{
    for (auto cell : buffer[row])
    {
        cell.set_color(0);
        cell.set_filled(false);
    }
}

void Playfield::drop_rows(int row)
{
    for (auto target_row = row - 1; target_row >= 0; target_row--)
    {
        for (auto cell : buffer[target_row])
        {
            Cell& target = get_cell(Coord(cell.get_coord(), Delta{1, 0}));
            target.set_filled(cell.get_filled());
            target.set_color(cell.get_color());
            cell.set_filled(false);
            cell.set_color(0);
        }
    }
}

vector<int> Playfield::clear_lines()
{
    vector<int> cleared;
    for (auto row = rows - 1, start = 0; row >= 0; row--)
    {
        if (row_filled(row))
        {
            clear_row(row);
            drop_rows(row);
            if (cleared.empty())
            {
                start = row;
                cleared.push_back(row);
            }
            else
            {
                cleared.push_back(--start);
            }
            row = rows;
        }
    }
    return cleared;
}

bool Playfield::game_over()
{
    for (auto cell : buffer[0])
    {
        if (cell.get_filled())
            return true;
    }
    return false;
}

void Playfield::draw(Coord origin, int shift)
{
    for (auto row = 0; row < rows; row++)
    {
        for (auto col = 0; col < cols; col++)
        {
            Cell& cell = buffer[row][col];
            bool filled = cell.get_filled();
            int color_pair = (cell.get_color() + shift) % NUM_TETROMINOS + 1;
            if (has_colors() && filled)
                attron(COLOR_PAIR((color_pair)));

            char symbol = cell.get_filled() ? '#' : ' ';
            Coord target = Coord(origin, Delta{ row, col });

            mvaddch(target.get_y(), target.get_x(), symbol);

            if (has_colors() && filled)
                attroff(COLOR_PAIR((color_pair)));
        }
    }
}
