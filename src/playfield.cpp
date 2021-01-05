/* The book I'm reading wasn't too clear on the ideal way to separate 
 * classes between .cpp and .hpp files. I will therefore start by
 * implementing it entirely in a .cpp file, and move things into the .hpp
 * file where it seems helpful or appropriate; or to see how that changes
 * or affects compilation.  */

/* This is an I/O class that is entirely concerned with representing the 
 * things which will be on the screen. For now it uses ncurses.  */
#include <ncurses.h>
#include <vector>

#include "playfield.hpp"

using std::vector;

class Coord {
    /* A symbol y,x coordinate class.  */
    public:
        Coord(int y, int x) : y(y), x(x) {}

        int get_y() {
            return y;
        }

        int get_x() {
            return x;
        }

    private:
        int y;
        int x;
};

class Cell {
    /* The Cell represents the attributes of a character cell within the playfield,
     * and is a place to collect information about what should be printed.  */
    public:
        Cell(int color, char symbol, Coord coord) : color(color), symbol(symbol), coord(coord) {}
   
        int get_color() {
            return color;
        }

        void set_color(int c) {
            color = c;
        }

        char get_symbol() {
            return symbol;
        }

        void set_symbol(char s) {
            symbol = s;
        }

        Coord get_coord() {
            return coord;
        }

    private:
        int color;
        char symbol;
        Coord coord;
};

class Playfield { 
     /* The Playfield represents the character-cell buffer that is analogous
     * to a pixel buffer. For now I am going to use the default terminal colors
     * only. I will integrate custom color pairs later. I suppose I should wrap the
     * the initialization of colors in here.  */
    public:
        Playfield(int rows, int cols) : rows(rows), cols(cols) {
            /* All cells are initially set to black background/white foreground with
             * a dot symbol.  */
            color = init_colors();
            for (auto row = 0; row < rows; row++) {
                vector<Cell> new_row;
                for (auto col = 0; col < cols; col++) {
                    new_row.push_back(Cell(BLACK_BG_WHITE_FG, '.', Coord(row, col)));
                }
                buffer.push_back(new_row);
            }
        }

        int get_rows() {
            return rows;
        }
    
        int get_cols() {
            return cols;
        }

        bool init_colors() {
            if (has_colors()) {
                init_pair(BLACK_BG_WHITE_FG, COLOR_WHITE, COLOR_BLACK);
                init_pair(BLACK_BG_RED_FG, COLOR_RED, COLOR_BLACK);
                init_pair(BLACK_BG_YELLOW_FG, COLOR_YELLOW, COLOR_BLACK);
                init_pair(BLACK_BG_GREEN_FG, COLOR_GREEN, COLOR_BLACK);
                init_pair(BLACK_BG_BLUE_FG, COLOR_BLUE, COLOR_BLACK);
                init_pair(BLACK_BG_CYAN_FG, COLOR_CYAN, COLOR_BLACK);
                init_pair(BLACK_BG_MAGENTA_FG, COLOR_MAGENTA, COLOR_BLACK);
                return true;
            } 
            return false;
        }

        void draw(Coord pos) {
            for (auto row = 0; row < rows; row++) {
                for (auto col = 0; col < cols; col++) {
                    int pair = buffer[row][col].get_color();
                    char symbol = buffer[row][col].get_symbol();

                    if (color)
                        attron(COLOR_PAIR(pair));

                    mvaddch(pos.get_y() + row, pos.get_x() + col, symbol);

                    if (color)
                        attroff(COLOR_PAIR(pair));
                }
            }
        }
 
    private:
        int rows;
        int cols;
        bool color;
        vector<vector<Cell>> buffer;
};

