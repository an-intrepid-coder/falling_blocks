#include "FallingBlocks.hpp"

using std::this_thread::sleep_for;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

using namespace std::chrono_literals;

FallingBlocks::FallingBlocks(int starting_level) : level(1), lines_cleared(0), score(0), lines_this_level(0)
{
    init_curses();

    if (starting_level > 1)
    {
        for (auto levels = starting_level - 1; levels > 0; levels--)
            level_up();
    }
}

FallingBlocks::~FallingBlocks()
{
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
    if (rows < GAME_HEIGHT || cols < GAME_WIDTH)
    {
        printw("Please ensure your terminal window is %d rows by %d columns or greater and try again. Any key to exit.", GAME_HEIGHT, GAME_WIDTH);
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
    tetromino.draw(playfield_origin);

    int hud_y = playfield_origin.get_y(), hud_x = playfield_origin.get_x() + PLAYFIELD_WIDTH + 1;
    mvprintw(hud_y, hud_x, "Level: %d", level);
    mvprintw(hud_y + 1, hud_x, "Lines: %d", lines_cleared);
    mvprintw(hud_y + 2, hud_x, "Score: %d", score);
    mvprintw(hud_y + 3, hud_x, "Next Up: ");

    Tetromino next = Tetromino(Coord(4, PLAYFIELD_WIDTH + 1), generator.preview());
    next.draw(playfield_origin);

    refresh();
}

void FallingBlocks::level_up()
{
    level++;

    lines_this_level %= LINES_TO_LEVEL;

    if (gravity_threshold.count() - MS_DECREMENT >= MS_FLOOR)
        gravity_threshold -= milliseconds{MS_DECREMENT};
}

void FallingBlocks::pause()
{
    nodelay(stdscr, false);
    draw_game();
    mvprintw(0, 0, "GAME PAUSED -- PRESS 'P' AGAIN TO CONTINUE");
    int input = convert_input(getch());
    while (input != MOVE_PAUSE) {}
    nodelay(stdscr, true);
    draw_game();
}

unsigned long int FallingBlocks::game_loop()
{
    while (!playfield.game_over())
    {
        if (tetromino.resting(playfield))
        {
            tetromino.freeze(playfield);
            tetromino = generator.next(playfield);
            unsigned long int cleared = playfield.clear_lines();
            lines_cleared += cleared;
            lines_this_level += cleared;
            score = cleared == 4 ? score + BONUS_SCORE : score + cleared * LINE_SCORE;
            if (lines_this_level >= LINES_TO_LEVEL)
                level_up();
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
            case MOVE_PAUSE:
                pause();
            break;
        }
        draw_game();

        sleep_for(FRAME_WAIT);
    }
    return score;
}
