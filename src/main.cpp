#include <chrono>
#include <thread>

#include "playfield.cpp"
#include "tetromino.cpp"


using std::this_thread::sleep_for;
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

    tetromino.draw(playfield_origin);

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

bool game_over(Tetromino& tetromino) {
    vector<Block> filled = tetromino.get_filled_blocks();
    for (Block block : filled) {
        if (block.get_coord().get_y() < 0)
            return true;
    }
    return false;
}

// Should I factor this into a class? I think so.
void falling_blocks() {
    TetrominoGenerator generator = TetrominoGenerator();
    Playfield playfield = Playfield(PLAYFIELD_HEIGHT, PLAYFIELD_WIDTH);
    // To-Do: Pre-rotate new tetrominos
    Tetromino tetromino = generator.next();

    auto gravity_threshold = 1000; // programatically reducible
    milliseconds gravity_threshold_ms{gravity_threshold};
    auto gravity_clock = high_resolution_clock::now();
    for (;;) {
        int input = convert_input(getch());

        duration<double, std::milli> elapsed = high_resolution_clock::now() - gravity_clock;
        if (elapsed > gravity_threshold_ms && input != MOVE_DOWN) { 
            tetromino.move(playfield, MOVE_DOWN);
            gravity_clock = high_resolution_clock::now();
        }

        if (input == MOVE_LEFT || input == MOVE_RIGHT || input == MOVE_DOWN) {
            tetromino.move(playfield, input);
        } else if (input == MOVE_ROTATE) {
            tetromino.rotate(playfield);
        }

        if (tetromino.resting(playfield)) {  
            if (game_over(tetromino))
                break;  // placeholder
            tetromino.freeze(playfield);
            tetromino = generator.next();
        }

        draw_game(playfield, tetromino);

        sleep_for(33ms); //hardcoded FPS for the moment
    }
}

int main() {
    init_curses();

    falling_blocks();

    uninit_curses();

    return 0;
}

