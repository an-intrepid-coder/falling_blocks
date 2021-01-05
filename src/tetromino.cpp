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

        void set_type(int new_type) {
            type = new_type;
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

        // The following functions are rough concepts and may need more parameters:
        void move() {
            // Will control down/left/right movement.
        }

        void rotate() {
            // Will control clockwise or counterclockwise rotation.
        }

        bool resting() {
            // Will return true if the tetromino is resting on at the bottom of the playfield
            // or on top of another tetromino. A "grace period" can also be built in here, to
            // replicate common functionality.
        }

        void freeze() {
            // "freezes" the tetromino to the playfield and deactivates it.
        }

    private:
        int type;
        int size;
        vector<vector<Block>> buffer;
};

