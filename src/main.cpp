#include <chrono>
#include <thread>
#include <random>

#include "playfield.cpp"
#include "tetromino.cpp"

using std::linear_congruential_engine;
using std::this_thread::sleep_for;
using std::chrono::system_clock;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

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
        if (coord.get_y() >= 0 && coord.get_y() < playfield.get_rows()) {
            mvaddch(playfield_origin.get_y() + coord.get_y(), 
                playfield_origin.get_x() + coord.get_x(), '#');
        }
    }
    attroff(COLOR_PAIR(tetromino.get_type()));

    refresh();
}

int convert_input(int input) {
    int return_value = MOVE_NONE;
    switch (input) {
        case 83:
        case 115:
            return_value = MOVE_DOWN;
        break;
        case 65:
        case 97:
            return_value = MOVE_LEFT;
        break;
        case 68:
        case 100:
            return_value = MOVE_RIGHT;
        break;
        case 70:
        case 102:
            return_value = MOVE_ROTATE;
        break;
        case 80:
        case 112:
            return_value = MOVE_PAUSE;
        break;
    }
    return return_value;
}

// Should I factor this into a class? I think so.
void falling_blocks() {
    unsigned long int seed = system_clock::to_time_t(system_clock::now());
    linear_congruential_engine<uint_fast32_t, 48271, 0, 2147483647> generator = 
        linear_congruential_engine<uint_fast32_t, 48271, 0, 2147483647>(seed);

    Playfield playfield = Playfield(PLAYFIELD_HEIGHT, PLAYFIELD_WIDTH);

    // To-Do: Pre-rotate new tetrominos
    Tetromino tetromino = Tetromino(Coord(START_ROW, generator() % START_COL_LIMIT),
                                    (generator() % NUM_TETROMINOS) + 1);

    auto gravity_threshold = 1000; // programatically reducible
    milliseconds gravity_threshold_ms{gravity_threshold};
    auto gravity_clock = high_resolution_clock::now();
    for (;;) {
        int input = convert_input(getch());

        duration<double, std::milli> elapsed = high_resolution_clock::now() - gravity_clock;
        if (elapsed > gravity_threshold_ms) {
            tetromino.move(playfield, MOVE_DOWN);
            gravity_clock = high_resolution_clock::now();
        }

        if (input == MOVE_LEFT || input == MOVE_RIGHT || input == MOVE_DOWN)
            tetromino.move(playfield, input);
/*
        if (tetromino.resting(playfield)) {
            tetromino.freeze(playfield);
        }
*/
        draw_game(playfield, tetromino);

        // debug
        mvprintw(0, 0, "%F", elapsed);
        vector<Block> filled = tetromino.get_filled_blocks();
        mvprintw(1, 0, "Num filled: %d", filled.size());
        //Coord test = filled[0].get_coord();
        //mvprintw(2, 0, "sample coord: (%d, %d)", test.get_y(), test.get_x());
        refresh();
        ///       
 
        sleep_for(33ms); //hardcoded FPS for the moment
    }
}

int main() {
    init_curses();

    falling_blocks();

    uninit_curses();

    return 0;
}

