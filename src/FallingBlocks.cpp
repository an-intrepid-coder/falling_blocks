#include "FallingBlocks.hpp"

using std::this_thread::sleep_for;
using std::chrono::high_resolution_clock;
using std::chrono::duration;
using std::chrono::milliseconds;

using namespace std::chrono_literals;

FallingBlocks::FallingBlocks(int starting_level, bool solid_color) : level(1), lines_cleared(0), score(0), lines_this_level(0)
{
    init_curses(solid_color);

    if (starting_level > 1)
    {
        for (auto levels = starting_level - 1; levels > 0; levels--)
            level_up();
    }

    getmaxyx(stdscr, term_height, term_width);
    generate_background();
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
        case 81:
            return_value = MOVE_QUIT;
        break;
    }
    return return_value;
}

void FallingBlocks::init_curses(bool solid_color)
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, true);
    if (has_colors())
    {
        start_color();
        init_pair(BLACK_BG_WHITE_FG, COLOR_WHITE, COLOR_BLACK);
        if (solid_color)
        {
            init_pair(SOLID_WHITE, COLOR_WHITE, COLOR_WHITE);
            init_pair(SOLID_RED, COLOR_RED, COLOR_RED);
            init_pair(SOLID_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
            init_pair(SOLID_GREEN, COLOR_GREEN, COLOR_GREEN);
            init_pair(SOLID_BLUE, COLOR_BLUE, COLOR_BLUE);
            init_pair(SOLID_CYAN, COLOR_CYAN, COLOR_CYAN);
            init_pair(SOLID_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);

        }
        else
        {
            init_pair(WHITE_ASCII, COLOR_WHITE, COLOR_BLACK);
            init_pair(RED_ASCII, COLOR_RED, COLOR_BLACK);
            init_pair(YELLOW_ASCII, COLOR_YELLOW, COLOR_BLACK);
            init_pair(GREEN_ASCII, COLOR_GREEN, COLOR_BLACK);
            init_pair(BLUE_ASCII, COLOR_BLUE, COLOR_BLACK);
            init_pair(CYAN_ASCII, COLOR_CYAN, COLOR_BLACK);
            init_pair(MAGENTA_ASCII, COLOR_MAGENTA, COLOR_BLACK);
        }
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

void FallingBlocks::generate_background()
{
    background = Playfield(term_height, term_width);

    for (auto tetrominos = 0; tetrominos < term_height * BACKGROUND_TETROMINOS_MULT; tetrominos++)
    {
        Tetromino bg_tetromino = generator.next(background);

        while (!bg_tetromino.resting(background))
        {
            bg_tetromino.attempt_move(background, MOVE_DOWN);
        }

        bg_tetromino.freeze(background);
    }
}

void FallingBlocks::draw_playfield_border(Coord origin)
{
    for (auto row = origin.get_y(); row < origin.get_y() + playfield.get_rows(); row++)
    {
        for (auto col = origin.get_x() - BORDER_WIDTH; col < origin.get_x(); col++)
        {
            char symbol = col == origin.get_x() - BORDER_WIDTH ? '|' : ' ';
            mvaddch(row, col, symbol);
        }

        int start_hudside = origin.get_x() + playfield.get_cols();
        int limit_hudside = row < origin.get_y() + HUD_LINES ? term_width : start_hudside + BORDER_WIDTH;

        for (auto col = start_hudside; col < limit_hudside; col++)
        {
            char symbol = row < origin.get_y() + HUD_LINES ? ' ' : col == limit_hudside - 1 ? '|' : ' ';
            mvaddch(row, col, symbol);
        }
    }

    if (term_height > playfield.get_rows())
    {
        for (auto col = origin.get_x() - BORDER_WIDTH; col < origin.get_x() + playfield.get_cols() + BORDER_WIDTH; col++)
            mvaddch(origin.get_y() + playfield.get_rows(), col, '#');
    }
}

void FallingBlocks::draw_game()
{
    erase();

    int max_height, max_width;
    getmaxyx(stdscr, max_height, max_width);

    if (max_height != term_height || max_width != term_width)
    {
        term_height = max_height;
        term_width = max_width;
        generate_background();
    }

    background.draw(Coord(0, 0), false);

    Coord playfield_origin = Coord(max_height / 2 - playfield.get_rows() / 2, max_width / 2 - playfield.get_cols() / 2);

    draw_playfield_border(playfield_origin);

    playfield.draw(playfield_origin, true);
    tetromino.draw(playfield_origin);

    int hud_y = playfield_origin.get_y(), hud_x = playfield_origin.get_x() + PLAYFIELD_WIDTH;
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

    generate_background();
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
            case MOVE_QUIT:
                return score;
            break;
        }
        draw_game();

        sleep_for(FRAME_WAIT);
    }
    return score;
}
