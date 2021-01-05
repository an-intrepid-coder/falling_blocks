#include <chrono>
#include <thread>

#include "playfield.cpp"
#include "tetromino.cpp"

using std::this_thread::sleep_for;

using namespace std::chrono_literals;

void init_curses (void) {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, true);
    if (has_colors()) {
        start_color();
    }
}

void uninit_curses (void) {
    echo();
    nocbreak();
    curs_set(1);
    nodelay(stdscr, false);
    endwin();
}

void draw_game(Playfield& playfield, Tetromino& tetromino) { 
    erase();

    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);

    Coord playfield_origin = Coord(max_height / 2 - playfield.get_rows() / 2,
                                   max_width / 2 - playfield.get_cols() / 2);

    playfield.draw(playfield_origin);

    attron(COLOR_PAIR(tetromino.get_type()));
    vector<Block> blocks = tetromino.get_filled_blocks();
    for (Block block : blocks) {
        Coord coord = block.get_coord();
        mvaddch(playfield_origin.get_y() + coord.get_y(), 
                playfield_origin.get_x() + coord.get_x(), '#');
    }
    attroff(COLOR_PAIR(tetromino.get_type()));

    refresh();
}

int main() {

    init_curses();

    Playfield playfield = Playfield(24, 10);
    Tetromino tetromino = Tetromino(Coord(5, 2), TETROMINO_L_B);

    draw_game(playfield, tetromino);
    sleep_for(3000ms);

    uninit_curses();

    return 0;
}

