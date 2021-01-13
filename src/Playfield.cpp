#include <ncurses.h>
#include "Playfield.hpp"

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
        cell.set_color(BLACK_BG_WHITE_FG);
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
            cell.set_color(BLACK_BG_WHITE_FG);
        }
    }
}

unsigned long int Playfield::clear_lines()
{
    unsigned long int cleared = 0;
    for (auto row = rows - 1; row >= 0; row--)
    {
        if (row_filled(row))
        {
            clear_row(row);
            drop_rows(row);
            row = rows - 1;
            cleared++;
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

void Playfield::draw(Coord origin)
{
    for (auto row = 0; row < rows; row++)
    {
        for (auto col = 0; col < cols; col++)
        {
            Cell& cell = buffer[row][col];
            if (has_colors())
                attron(COLOR_PAIR(cell.get_color()));

            char symbol = cell.get_filled() ? '#' : '.';
            Coord target = Coord(origin, Delta{ row, col });

            mvaddch(target.get_y(), target.get_x(), symbol);

            if (has_colors())
                attroff(COLOR_PAIR(cell.get_color()));
        }
    }
}
