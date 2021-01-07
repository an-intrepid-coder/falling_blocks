#include <vector>
#include <deque>
#include <random>
#include <ncurses.h>

#include "tetromino.hpp"
#include "playfield.hpp"

using std::vector;
using std::deque;
using std::chrono::system_clock;
using std::linear_congruential_engine;

class Block {
    public:
        Block(Coord coord, int type) : coord(coord), type(type) {
            filled = false;
        }

        Coord get_coord() {
            return coord;
        }

        void set_coord(Coord new_coord) {
            coord = new_coord;
        }

        int get_type() {
            return type;
        }

        bool get_filled() {
            return filled;
        }

        void set_filled(bool set) {
            filled = set;
        }

    private:
        Coord coord;
        int type;
        bool filled;
};

class Tetromino {
    class MoveDirs {
        public:
            MoveDirs(int dir) {
                switch(dir) {
                    case MOVE_LEFT:
                        dy = 0;
                        dx = -1;
                    break;
                    case MOVE_RIGHT:
                        dy = 0;
                        dx = 1;
                    break;
                    case MOVE_DOWN:
                        dy = 1;
                        dx = 0;
                    break;
                    default:
                        dy = 0;
                        dx = 0;
                    break;
                }
            }

            int get_dy() {
                return dy;
            }

            int get_dx() {
                return dx;
            }

        private:
            int dy;
            int dx;
    };

    public:
        Tetromino(Coord origin, int type) : type(type) {
            if (type == TETROMINO_STRAIGHT) {
                size = 4;
            } else if (type == TETROMINO_SQUARE) {
                size = 2;
            } else {
                size = 3;
            }

            for (auto row = 0; row < size; row++) {
                vector<Block> new_row;
                for (auto col = 0; col < size; col++) {
                    new_row.push_back(Block(Coord(origin.get_y() + row, 
                                            origin.get_x() + col), 
                                      type));
                }
                buffer.push_back(new_row);
            }

            switch (type) {
                case TETROMINO_STRAIGHT:
                    for (auto block = 0; block < size; block++)
                        buffer[block][0].set_filled(true);
                break;
                case TETROMINO_SQUARE:
                    buffer[0][0].set_filled(true);
                    buffer[0][1].set_filled(true);
                    buffer[1][0].set_filled(true);
                    buffer[1][1].set_filled(true);
                break;
                case TETROMINO_SKEW_A:
                    buffer[2][0].set_filled(true);
                    buffer[2][1].set_filled(true);
                    buffer[1][1].set_filled(true);
                    buffer[1][2].set_filled(true);
                break;
                case TETROMINO_SKEW_B:
                    buffer[1][0].set_filled(true);
                    buffer[1][1].set_filled(true);
                    buffer[2][1].set_filled(true);
                    buffer[2][2].set_filled(true);
                break;
                case TETROMINO_L_A:
                    buffer[0][0].set_filled(true);
                    buffer[1][0].set_filled(true);
                    buffer[2][0].set_filled(true);
                    buffer[2][1].set_filled(true);
                break;
                case TETROMINO_L_B:
                    buffer[0][2].set_filled(true);
                    buffer[1][2].set_filled(true);
                    buffer[2][2].set_filled(true);
                    buffer[2][1].set_filled(true);
                break;
                case TETROMINO_T:
                    buffer[1][1].set_filled(true);
                    buffer[2][0].set_filled(true);
                    buffer[2][1].set_filled(true);
                    buffer[2][2].set_filled(true);
                break;
            }
        }

        int get_type() {
            return type;
        }

        int get_size() {
            return size;
        }

        vector<Block> get_filled_blocks() { 
            vector<Block> filled;
            for (auto row = 0; row < size; row++) {
                for (auto col = 0; col < size; col++) {
                    if (buffer[row][col].get_filled())
                        filled.push_back(buffer[row][col]);
                }
            }
            return filled;
        }

        void move(Playfield& playfield, int dir) {
            bool valid = true;
            MoveDirs deltas = Tetromino::MoveDirs(dir);
            vector<Block> filled = get_filled_blocks();

            for (Block block : filled) {
                Coord old_coord = block.get_coord();
                Coord new_coord = Coord(old_coord.get_y() + deltas.get_dy(),
                                        old_coord.get_x() + deltas.get_dx());

                if (new_coord.get_x() < 0 || new_coord.get_x() >= playfield.get_cols()) {
                    valid = false;
                    break;
                }

                if (new_coord.get_y() >= 0) {
                    Cell& target = playfield.get_cell(new_coord);
                    if (target.get_filled()) {
                        valid = false;
                        break;
                    }
                }
            }

            if (valid) {
                for (vector<Block>& row : buffer) {
                    for (Block& block : row) {
                        Coord old_coord = block.get_coord();
                        block.set_coord(Coord(old_coord.get_y() + deltas.get_dy(),
                                              old_coord.get_x() + deltas.get_dx()));
                    }
                }
            }
        }

        // Well, that's almost perfect and it's about 10x shorter than my last version!
        /* Remaining challenges:
         *  1. "wobble", can be fixed with an offset probably. Looks silly for now.
         *     The issue is that they should appear to spin about around the bottom-
         *     center-ish of the tetromino, rather than the center of an invisible tile.
         *     Easily fixed on a per-tetromino-basis, but I am interested in general
         *     solutions. The most elegant solution may be to use a uniformly-sized tile
         *     for each block, with the tetromino at the center. Exactly like a sprite.  */
        void rotate(Playfield& playfield) {
            if (type == TETROMINO_SQUARE)
                return;

            vector<vector<Block>> new_buffer;
            for (vector<Block> row : buffer) {
                vector<Block> new_row;
                for (Block block : row) {
                    new_row.push_back(Block(block.get_coord(), type));
                }
                new_buffer.push_back(new_row);
            }
            
            bool valid = true;
            for (int row = 0; row < (int) new_buffer.size(); row++) {
                for (int col = 0; col < (int) new_buffer.size(); col++) {
                    int dy = 0 - row + col, dx = new_buffer.size() - 1 - row - col;

                    //aha
                    Coord target_coord = buffer[row + dy][col + dx].get_coord();
                    if (target_coord.get_y() >= 0) {                    
                        Cell& target = playfield.get_cell(target_coord);
                        if (target.get_filled()) {
                            valid = false;
                            break;
                        }
                    }

                    if (buffer[row][col].get_filled())
                        new_buffer[row + dy][col + dx].set_filled(true);
                }
            }
            if (valid)
                buffer = new_buffer;
        }

        bool resting(Playfield& playfield) {
            /* TO-DO: Add a timer for the "grace-period" that lets you slide the block
             * for just a split second before it freezes into place.  */
            vector<Block> filled = get_filled_blocks();
            for (Block block : filled) {
                Coord coord = block.get_coord();
                if (coord.get_y() == playfield.get_rows() - 1)
                    return true;

                if (coord.get_y() >= -1 && coord.get_y() < playfield.get_rows() - 1) {
                    Cell& cell = playfield.get_cell(Coord(coord.get_y() + 1, coord.get_x()));
                    if (cell.get_filled())
                        return true;
                }
            }
            return false;
        }

        void freeze(Playfield& playfield) {
            vector<Block> filled = get_filled_blocks();
            for (Block block : filled) {
                Cell& cell = playfield.get_cell(block.get_coord());
                cell.set_filled(true);
                cell.set_color(type);
            }
        }

        void draw(Coord origin) {
            attron(COLOR_PAIR(type));
            vector<Block> blocks = get_filled_blocks();
            for (Block block : blocks) {
                Coord coord = block.get_coord();
                if (coord.get_y() >= 0) {
                    mvaddch(origin.get_y() + coord.get_y(), 
                            origin.get_x() + coord.get_x(), '#');
                }
            }
            attroff(COLOR_PAIR(type));
        }

    private:
        int type;
        int size;
        vector<vector<Block>> buffer;
};

class TetrominoGenerator {
    public:
        TetrominoGenerator() {
            unsigned long int seed = system_clock::to_time_t(system_clock::now());
            generator = linear_congruential_engine<uint_fast32_t, 48271, 0, 2147483647>(seed);
            new_batch();
        }

        void new_batch() {
            deque<int> new_permutations;
            for (auto index = 1; index <= NUM_TETROMINOS; index++) {
                new_permutations.push_back(index);
            }
            for (auto shuffle = 0; shuffle < GENERATOR_SHUFFLES; shuffle++) {
                int i = generator() % NUM_TETROMINOS, j = generator() % NUM_TETROMINOS;
                int temp = new_permutations[i];
                new_permutations[i] = new_permutations[j];
                new_permutations[j] = temp;
            }
            permutations = new_permutations;
        }

        int preview() {
            return permutations.front();
        }

        Tetromino next() {
            int row = START_ROW;
            if (permutations.front() == TETROMINO_SQUARE) {
                row += 2;
            } else if (permutations.front() != TETROMINO_STRAIGHT) {
                row ++;
            }

            Tetromino next = Tetromino(Coord(row, generator() % START_COL_LIMIT), 
                                       permutations.front());

            permutations.pop_front();
            if (permutations.empty())
                new_batch();

            return next;
        }

    private:
        linear_congruential_engine<uint_fast32_t, 48271, 0, 2147483647> generator;
        deque<int> permutations;
};

