#include "FallingBlocks.hpp"

using std::this_thread::sleep_for;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

using namespace std::chrono_literals;

FallingBlocks::FallingBlocks() : level(1)
{
    init_curses();
}

FallingBlocks::~FallingBlocks()
{
    endwin();
    uninit_curses();
}

int FallingBlocks::convert_input(int input)
{
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

void FallingBlocks::init_curses()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, true);
    if (has_colors())
    {
        start_color(); // To-Do: Re-implement multiple color options
        init_pair(BLACK_BG_WHITE_FG, COLOR_WHITE, COLOR_BLACK);
        init_pair(BLACK_BG_RED_FG, COLOR_RED, COLOR_BLACK);
        init_pair(BLACK_BG_YELLOW_FG, COLOR_YELLOW, COLOR_BLACK);
        init_pair(BLACK_BG_GREEN_FG, COLOR_GREEN, COLOR_BLACK);
        init_pair(BLACK_BG_BLUE_FG, COLOR_BLUE, COLOR_BLACK);
        init_pair(BLACK_BG_CYAN_FG, COLOR_CYAN, COLOR_BLACK);
        init_pair(BLACK_BG_MAGENTA_FG, COLOR_MAGENTA, COLOR_BLACK);

    }
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    if (rows < 24 || cols < 10)  // Will need more when I re-implement the HUD
    {
        printw("Please ensure your terminal window is 24 rows by 10 columns or greater and try again. Any key to exit.");
        nodelay(stdscr, false);
        getch();
        endwin();
        exit(0);
    }
}

void FallingBlocks::uninit_curses()
{
    echo();
    nocbreak();
    curs_set(1);
    nodelay(stdscr, false);
    endwin();
}

void FallingBlocks::draw_game()
{
    erase();

    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);

    Coord playfield_origin = Coord(max_height / 2 - playfield.get_rows() / 2, max_width / 2 - playfield.get_cols() / 2);

    playfield.draw(playfield_origin);

    tetromino.draw(playfield_origin, playfield);

    refresh();
}

void FallingBlocks::game_loop()
{
    while (!playfield.game_over())
    {
        if (tetromino.resting(playfield))
        {
            tetromino.freeze(playfield);
            tetromino = generator.next(playfield);
            playfield.clear_lines();
        }

        duration<double, std::milli> elapsed = high_resolution_clock::now() - gravity_clock;
        if (elapsed > gravity_threshold)
        {
            tetromino.attempt_move(playfield, MOVE_DOWN);
            gravity_clock = high_resolution_clock::now();
        }

        int input = convert_input(getch());
        switch (input)
        {
            case MOVE_ROTATE:
                tetromino.clockwise_rotation(playfield);
            break;
            case MOVE_LEFT:
            case MOVE_RIGHT:
            case MOVE_DOWN:
                tetromino.attempt_move(playfield, input);
            break;
        }
        draw_game();

        sleep_for(33ms);
    }
}
