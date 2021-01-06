#include <vector>

#include "tetromino.hpp"
#include "playfield.hpp"

using std::vector;

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

        bool in_play(Playfield& playfield) {
            for (vector<Block>& row : buffer) {
                for (Block& block : row) {
                    Coord coord = block.get_coord();
                    if (coord.get_y() < 0 || coord.get_y() >= playfield.get_rows())
                        return false;
                }
            }
            return true;
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

                if (in_play(playfield)) {
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

        // The following functions are rough concepts and may need more parameters:
        void rotate() {
            // Will handle rotation. Not only will this need to rotate the "sprite"-like
            // tetromino object, but it will also need to account for the potential
            // "wobble" effect.
        }

        bool resting(Playfield& playfield) {
            vector<Block> filled = get_filled_blocks();
            for (Block block : filled) {
                Coord coord = block.get_coord();
                if (coord.get_y() == playfield.get_rows() - 1)
                    return true;

                if (coord.get_y() > 0 || coord.get_y() < playfield.get_rows() - 1) {
                    Cell& cell = playfield.get_cell(Coord(coord.get_y(), coord.get_x() + 1));
                    if (cell.get_filled())
                        return true;
                }
            }
            return false;
        }

        void freeze() {
            // "freezes" the tetromino to the playfield and deactivates it.
        }

    private:
        int type;
        int size;
        vector<vector<Block>> buffer;
};

