#include <chrono>
#include <thread>

#include "playfield.cpp"

using std::this_thread::sleep_for;

using namespace std::chrono_literals;

void init_curses (void) {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, true);
}

void uninit_curses (void) {
    echo();
    nocbreak();
    curs_set(1);
    nodelay(stdscr, false);
    endwin();
}

void draw_game(Playfield playfield) { 
    erase();

    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);

    // Making it a const reference seems to prevent me from using getters. That suggests
    // I'm missing a step, as surely using getters on a const reference is okay. Something
    // to look into later. I should re-read the chapter that covered this, and also
    // search for a more comprehensive source. This is an important aspect of the
    // language to get right.
    Coord playfield_origin = Coord(max_height / 2 - playfield.get_rows() / 2,
                                   max_width / 2 - playfield.get_cols() / 2);

    playfield.draw(playfield_origin);

    refresh();
}

int main() {

    init_curses();

    Playfield testPlayfield = Playfield(24, 10);

    draw_game(testPlayfield);
    sleep_for(3000ms);

    uninit_curses();

    return 0;
}

