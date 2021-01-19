#include <ncurses.h>
#include "Tetromino.hpp"
#include "Playfield.hpp"
#include "Cell.hpp"

using std::vector;

Tetromino::Tetromino(Coord origin, int type, bool ghost) : type(type), orientation(0), ghost(ghost)
{
    field_size = type == TETROMINO_SQUARE ? 2 : 5;

    for (auto row = 0; row < field_size; row++)
    {
        vector<Block> new_row;
        for (auto block = 0; block < field_size; block++)
            new_row.push_back(Block(Coord(origin, Delta{row, block}), type));

        blocks.push_back(new_row);
    }

    switch (type)
    {
        case TETROMINO_L_A:
            blocks[1][1].set_filled(true);
            blocks[2][1].set_filled(true);
            blocks[3][1].set_filled(true);
            blocks[3][2].set_filled(true);
        break;
        case TETROMINO_L_B:
            blocks[1][1].set_filled(true);
            blocks[2][1].set_filled(true);
            blocks[3][1].set_filled(true);
            blocks[3][0].set_filled(true);
        break;
        case TETROMINO_SKEW_A:
            blocks[1][2].set_filled(true);
            blocks[1][3].set_filled(true);
            blocks[2][1].set_filled(true);
            blocks[2][2].set_filled(true);
        break;
        case TETROMINO_SKEW_B:
            blocks[1][1].set_filled(true);
            blocks[1][2].set_filled(true);
            blocks[2][2].set_filled(true);
            blocks[2][3].set_filled(true);
        break;
        case TETROMINO_SQUARE:
            blocks[0][0].set_filled(true);
            blocks[0][1].set_filled(true);
            blocks[1][0].set_filled(true);
            blocks[1][1].set_filled(true);
        break;
        case TETROMINO_STRAIGHT:
            blocks[0][2].set_filled(true);
            blocks[1][2].set_filled(true);
            blocks[2][2].set_filled(true);
            blocks[3][2].set_filled(true);
        break;
        case TETROMINO_T:
            blocks[1][1].set_filled(true);
            blocks[2][0].set_filled(true);
            blocks[2][1].set_filled(true);
            blocks[2][2].set_filled(true);
        break;
    }
}

vector<vector<Block>> Tetromino::fresh_blocks()
{
    vector<vector<Block>> new_blocks;
    for (auto row : blocks)
    {
        vector<Block> new_row;
        for (auto block : row)
            new_row.push_back(Block(block.get_coord(), type));

        new_blocks.push_back(new_row);
    }
    return new_blocks;
}

bool Tetromino::valid_placement(Playfield& playfield, Block& old_block, Coord& target)
{
    if (old_block.get_filled() && !target.in_bounds(playfield.get_rows(), playfield.get_cols()))
        return false;

    if (old_block.get_filled() && target.in_bounds(playfield.get_rows(), playfield.get_cols()) && target.get_y() >= 0)
    {
        Cell& target_cell = playfield.get_cell(target);
        if (target_cell.get_filled())
            return false;
    }
    return true;
}

bool Tetromino::resting(Playfield& playfield)
{
    for (auto row : blocks)
    {
        for (auto block : row)
        {
            if (block.get_filled())
            {
                Coord coord = block.get_coord();
                if (coord.get_y() == playfield.get_rows() - 1)
                {
                    return true;
                }
                else if (coord.get_y() >= -1 && playfield.get_cell(Coord(coord, Delta{1, 0})).get_filled())
                {
                    return true;
                }
            }
        }
    }
    return false;
}

void Tetromino::freeze(Playfield& playfield)
{
    for (auto row : blocks)
    {
        for (auto block : row)
        {
            Coord coord = block.get_coord();
            if (block.get_filled() && coord.in_bounds(playfield.get_rows(), playfield.get_cols()) && coord.get_y() >= 0)
            {
                Cell &target = playfield.get_cell(block.get_coord());
                target.set_filled(true);
                target.set_color(type);
            }
        }
    }
}

void Tetromino::attempt_move(Playfield& playfield, int move_type)
{
    Delta dir;
    switch(move_type)
    {
        case MOVE_DOWN:
            dir.dx = 0;
            dir.dy = 1;
        break;
        case MOVE_LEFT:
            dir.dx = -1;
            dir.dy = 0;
        break;
        case MOVE_RIGHT:
            dir.dx = 1;
            dir.dy = 0;
        break;
        default:
            dir.dx = 0;
            dir.dy = 0;
        break;
    }

    vector<vector<Block>> new_blocks = fresh_blocks();
    bool valid = true;
    for (auto row = 0; row < field_size; row++)
    {
        for (auto block = 0; block < field_size; block++)
        {
            Block old_block = blocks[row][block];
            Coord target = Coord(old_block.get_coord(), dir);
            if (!valid_placement(playfield, old_block, target))
            {
                valid = false;
                break;
            }
            new_blocks[row][block].set_coord(target);
            new_blocks[row][block].set_filled(old_block.get_filled());
        }
    }

    if (valid)
        blocks = new_blocks;
}

void Tetromino::clockwise_rotation(Playfield& playfield)
{
    if (type == TETROMINO_SQUARE)
        return;

    vector<vector<Block>> new_blocks = fresh_blocks();

    bool valid = true;
    for (auto row = 0; row < field_size; row++)
    {
        for (auto block = 0; block < field_size; block++)
        {
            Block old_block = blocks[row][block];
            if (old_block.get_filled())
            {
                int dy = 0 - row + block, dx = field_size - 1 - row - block;
                offset(dy, dx);
                new_blocks[row + dy][block + dx].set_filled(true);
                Coord target = new_blocks[row + dy][block + dx].get_coord();
                if (!valid_placement(playfield, old_block, target) && !ghost)
                {
                    valid = false;
                    break;
                }
            }
        }
    }

    if (valid)
    {
        blocks = new_blocks;
        orientation = (orientation + 90) % 360;
    }
}

void Tetromino::offset(int& dy, int& dx)
{
    switch(type)
    {
        case TETROMINO_L_A:
        case TETROMINO_L_B:
        case TETROMINO_T:
            dx--;
            dy++;
        break;
        case TETROMINO_SKEW_A:
        case TETROMINO_SKEW_B:
            switch (orientation)
            {
                case 0:
                case 180:
                    dx--;
                    dy--;
                break;
                case 90:
                case 270:
                    dx--;
                break;
            }
        break;
        case TETROMINO_STRAIGHT:
            switch (orientation)
            {
                case 0:
                case 180:
                    dx--;
                break;
            }
        break;
    }
}

void Tetromino::draw(Coord origin, int shift)
{
    for (auto row : blocks)
    {
        for (auto block : row)
        {
            Coord coord = block.get_coord();
            int color_pair = (type + shift) % NUM_TETROMINOS + 1;
            char symbol = ghost ? '*' : '#';
            if (block.get_filled() && coord.get_y() >= 0)
            {
                if (has_colors() && !ghost)
                    attron(COLOR_PAIR(color_pair));

                Coord coord = block.get_coord();
                Coord target = Coord(origin, Delta{coord.get_y(), coord.get_x()});
                mvaddch(target.get_y(), target.get_x(), symbol);

                if (has_colors() && !ghost)
                    attroff(COLOR_PAIR(color_pair));
            }
        }
    }
}

