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

void falling_blocks() {
    Playfield playfield = Playfield(24, 10);
    // To-Do: Randomized batch-based tetromino-generation:
    Tetromino tetromino = Tetromino(Coord(0, 0), TETROMINO_L_B);

    //naive loop then I'll add/cut as needed for a cleaner version 
    for (;;) {

        int input = convert_input(getch());

        if (input == MOVE_LEFT || input == MOVE_RIGHT || input == MOVE_DOWN) {
            tetromino.move(playfield, input);
        }

        draw_game(playfield, tetromino);
        
        // debug: The block is not actually staying moved!
        vector<Block> filled = tetromino.get_filled_blocks();
        Coord filled_coord = filled[0].get_coord();
        mvprintw(0, 0, "A Filled Block: (%d, %d)", 
                 filled_coord.get_y(), filled_coord.get_x());
        refresh();
        ///
        sleep_for(33ms); //hardcoded FPS for the moment
    }
}

// Next task should be the game loop, I suppose. This part will be the most
// different, aside from rotation itself, due to the OOP changes of C++. I suppose
// I could even make a game object; I should consider that as I go. That's basically
// what the stats object was.
int main() {
    init_curses();

    falling_blocks();

    uninit_curses();

    return 0;
}

