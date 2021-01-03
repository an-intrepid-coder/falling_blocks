#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "falling_blocks.h"
#include "tetromino.c"
#include "playfield.c"
#include "stats.c"

bool game_fits_screen (void) {
    int terminal_height, terminal_width;
    getmaxyx(stdscr, terminal_height, terminal_width);
    if (terminal_height >= GAME_SCREEN_HEIGHT && terminal_width >= GAME_SCREEN_WIDTH) {
        return true;
    } 
    return false;
}

Coord get_playfield_starting_yx (void) {
    Coord new_coord;

    int terminal_height, terminal_width;
    getmaxyx(stdscr, terminal_height, terminal_width);
    new_coord.y = (terminal_height / 2) - (GAME_SCREEN_HEIGHT / 2);
    new_coord.x = (terminal_width / 2) - (GAME_SCREEN_WIDTH / 2);

    return new_coord;
}

Block_Attr get_block_attr(int color_mode, int fill_type) {
    Block_Attr new_block;
    new_block.symbol = ' ';
    switch (color_mode) {
        case MODE_NO_COLOR:
            new_block.symbol = fill_type ? '#' : '.';
        break;
        case MODE_ASCII_COLOR:
            switch (fill_type) {
                case FILL_EMPTY:
                    new_block.pair = PAIR_NORMAL;
                    new_block.symbol = '.';
                break;
                case FILL_SQUARE: 
                    new_block.pair = PAIR_SQUARE_ASCII;
                    new_block.symbol = '#';
                break;
                case FILL_STRAIGHT:
                    new_block.pair = PAIR_STRAIGHT_ASCII;
                    new_block.symbol = '#';
                break;
                case FILL_SKEW_A:
                    new_block.pair = PAIR_SKEW_A_ASCII;
                    new_block.symbol = '#';
                break;
                case FILL_SKEW_B:
                    new_block.pair = PAIR_SKEW_B_ASCII;
                    new_block.symbol = '#';
                break;
                case FILL_L_A:
                    new_block.pair = PAIR_L_A_ASCII;
                    new_block.symbol = '#';
                break;
                case FILL_L_B:
                    new_block.pair = PAIR_L_B_ASCII;
                    new_block.symbol = '#';
                break;
                case FILL_T:
                    new_block.pair = PAIR_T_ASCII;
                    new_block.symbol = '#';
                break;
            }
        break;
        case MODE_SOLID_COLOR:
            switch (fill_type) {
                case FILL_EMPTY:
                    new_block.pair = PAIR_NORMAL;
                    new_block.symbol = '.';
                break;
                case FILL_SQUARE: 
                    new_block.pair = PAIR_SQUARE_SOLID;
                break;
                case FILL_STRAIGHT:
                    new_block.pair = PAIR_STRAIGHT_SOLID;
                break;
                case FILL_SKEW_A:
                    new_block.pair = PAIR_SKEW_A_SOLID;
                break;
                case FILL_SKEW_B:
                    new_block.pair = PAIR_SKEW_B_SOLID;
                break;
                case FILL_L_A:
                    new_block.pair = PAIR_L_A_SOLID;
                break;
                case FILL_L_B:
                    new_block.pair = PAIR_L_B_SOLID;
                break;
                case FILL_T:
                    new_block.pair = PAIR_T_SOLID;
                break;
            }
        break;
    }

    return new_block;
}

void draw_playfield (Playfield *playfield, int color_mode) {
    Coord starting_yx = get_playfield_starting_yx();
    for (int row = 0; row < PLAYFIELD_HEIGHT; row++) {
        for (int cell = 0; cell < PLAYFIELD_WIDTH; cell++) {
            Block_Attr next_block = get_block_attr(color_mode, 
                                                   playfield->fill_type[row][cell]);
            attron(COLOR_PAIR(next_block.pair));
            mvaddch(starting_yx.y + row, starting_yx.x + cell, next_block.symbol);
            attroff(COLOR_PAIR(next_block.pair));
        }
    }
}

void draw_next(int y, int x, int color_mode, Permutations_List *plist) {
    /* Draws a representation of the next tetromino.  */
    for (int row = 0; row < NEXT_HEIGHT; row++) {
        for (int col = 0; col < NEXT_WIDTH; col++) {
            Block_Attr next_block = get_block_attr(color_mode, plist->next_buffer[row][col]);
            attron(COLOR_PAIR(next_block.pair));
            mvaddch(y + row, x + col, next_block.symbol);
            attroff(COLOR_PAIR(next_block.pair));
        }
    }
}

void draw_hud (Stats *stats, int color_mode, Permutations_List *plist) {
    Coord starting_yx = get_playfield_starting_yx();
    int starting_x = starting_yx.x + PLAYFIELD_WIDTH;

    mvprintw(starting_yx.y + SCORE_LINE, starting_x, "Score: %d", stats->score);
    mvprintw(starting_yx.y + LEVEL_LINE, starting_x, "Level: %d", stats->level);
    mvprintw(starting_yx.y + NEXT_LINE, starting_x, "Next Tetromino:");
    draw_next(starting_yx.y + NEXT_LINE + 1, starting_x + 4, color_mode, plist);
}

void draw_game (Playfield *playfield, Stats *stats, Permutations_List *plist, 
                int color_mode) {
    static bool init = true;
    static int last_rows, last_cols;
    int rows, cols;

    erase(); 

    if (init) {
        getmaxyx(stdscr, last_rows, last_cols);
        init = false;
    } 

    getmaxyx(stdscr, rows, cols);

    if (rows != last_rows || cols != last_cols) {
        getmaxyx(stdscr, last_rows, last_cols);
        erase();
    }

    draw_playfield(playfield, color_mode);
    draw_hud(stats, color_mode, plist);
}

// A couple of curses wrappers
void init_curses (void) {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    nodelay(stdscr, true);
    if (has_colors()) {
        start_color();
        init_pair(PAIR_NORMAL, COLOR_WHITE, COLOR_BLACK);
        init_pair(PAIR_SQUARE_ASCII, COLOR_WHITE, COLOR_BLACK);
        init_pair(PAIR_STRAIGHT_ASCII, COLOR_CYAN, COLOR_BLACK);
        init_pair(PAIR_SKEW_A_ASCII, COLOR_RED, COLOR_BLACK);
        init_pair(PAIR_SKEW_B_ASCII, COLOR_BLUE, COLOR_BLACK);
        init_pair(PAIR_L_A_ASCII, COLOR_GREEN, COLOR_BLACK);
        init_pair(PAIR_L_B_ASCII, COLOR_YELLOW, COLOR_BLACK);
        init_pair(PAIR_T_ASCII, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(PAIR_SQUARE_SOLID, COLOR_WHITE, COLOR_WHITE);
        init_pair(PAIR_STRAIGHT_SOLID, COLOR_CYAN, COLOR_CYAN);
        init_pair(PAIR_SKEW_A_SOLID, COLOR_RED, COLOR_RED);
        init_pair(PAIR_SKEW_B_SOLID, COLOR_BLUE, COLOR_BLUE);
        init_pair(PAIR_L_A_SOLID, COLOR_GREEN, COLOR_GREEN);
        init_pair(PAIR_L_B_SOLID, COLOR_YELLOW, COLOR_YELLOW);
        init_pair(PAIR_T_SOLID, COLOR_MAGENTA, COLOR_MAGENTA);
    }
}

void uninit_curses (void) {
    echo();
    nocbreak();
    curs_set(1);
    nodelay(stdscr, false);
    endwin();
}

int convert_input (int input) {
    /* Is passed the ASCII value of the user input and returns an enum.  */
    int return_value = DIRECTION_NONE;
    switch (input) {
        case 83:
        case 115:
            return_value = DOWN;
        break;
        case 65:
        case 97:
            return_value = LEFT;
        break;
        case 68:
        case 100:
            return_value = RIGHT;
        break;
        case 70:
        case 102:
            return_value = ROTATE;
        break;
        case 80:
        case 112:
            return_value = PAUSE;
        break;
        default:
            return_value = DIRECTION_NONE;
        break;
    }
    return return_value;
}

double get_elapsed (struct timespec *start) {
    struct timespec end;

    clock_gettime(CLOCK_REALTIME, &end);

    double start_secs, end_secs;
    start_secs = start->tv_sec + start->tv_nsec / (double) NSECS_PER_SEC;
    end_secs = end.tv_sec + end.tv_nsec / (double) NSECS_PER_SEC;

    return end_secs - start_secs;
}

bool state_timer_reached (Stats *stats, struct timespec *state_timer) {

    double elapsed = get_elapsed(state_timer);

    if (elapsed >= stats->step_interval) {
        clock_gettime(CLOCK_REALTIME, state_timer);
        return true;
    }

    return false;
}

void frame_wait(struct timespec *loop_timer) {
    double delay = get_elapsed(loop_timer);

    struct timespec wait = {
        0,
        (long int) ((FRAME_INTERVAL - delay) * NSECS_PER_SEC),
    };

    nanosleep(&wait, NULL); 
}

void game_over () {
    /* Turns off non-blocking mode and alerts the player to exit the game.  */
    nodelay(stdscr, false);
    mvprintw(0, 0, "Game Over!");
    mvprintw(1, 0, " ... 'q' key to exit.");
    while (getch() != 113) {}
}

void pause_game (Stats *stats) {
    nodelay(stdscr, false);
    mvprintw(0, 0, "GAME PAUSED");
    mvprintw(1, 0, "...any key to continue...");
    getch();
    stats->paused = false;
    erase();
    nodelay(stdscr, true);
}

void print_debug (Stats *stats, Tetromino *tetromino, 
                  Permutations_List *plist, double last_frame) {
    /* Prints all kinds of information about the state of the program.  */
    char buffer[150] = {0}, buffer2[5] = {0};
    mvprintw(3, 0, "ticks: %ld", stats->ticks);
    mvprintw(4, 0, "step/tick interval: %F", stats->step_interval);
    mvprintw(5, 0, "current tetromino type: %d", tetromino->tetromino_type);
    mvprintw(6, 0, "current tetromino configuration: %d", tetromino->tetromino_configuration);

    sprintf(buffer, "block coords (y, x): (%d, %d), (%d, %d), (%d, %d), (%d, %d)", 
            tetromino->blocks_yx[0].y, tetromino->blocks_yx[0].x, tetromino->blocks_yx[1].y, 
            tetromino->blocks_yx[1].x, tetromino->blocks_yx[2].y, tetromino->blocks_yx[2].x, 
            tetromino->blocks_yx[3].y, tetromino->blocks_yx[3].x);
    mvprintw(7, 0, "%s", buffer);

    mvprintw(8, 0, "permutation list length: %d", plist->length);
    buffer[0] = '{';
    buffer[1] = ' ';
    int index = 2;
    for (int count = 0; count < plist->length; count++) {
        sprintf(buffer2, "%d", plist->types[count]);
        buffer[index++] = buffer2[0];
        buffer[index++] = ' ';
    }
    buffer[index++] = '}';
    buffer[index++] = '\0';
    mvprintw(9, 0, "permutation list: %s", buffer);

    mvprintw(10, 0, "time since last frame(seconds): %F", last_frame);
}

void a_game_of_falling_blocks (int difficulty_level, int color_mode, int debug) {
    srand((unsigned) time(NULL));

    Playfield playfield = playfield_constructor();
    Stats stats = stats_constructor();

    Permutations_List plist = permutations_list_constructor();
    int tetromino_topleft_x = rand() % SPAWN_LIMIT, next = next_tetromino(&plist);
    Tetromino tetromino = tetromino_constructor(next, tetromino_topleft_x, &playfield);
    next = next_tetromino(&plist);

    struct timespec state_timer, loop_timer;
    clock_gettime(CLOCK_REALTIME, &state_timer);
    while (!tetromino.game_over) {
        int input;
        double since_last_frame;
        if (debug) {
            since_last_frame = get_elapsed(&loop_timer);
        }
        clock_gettime(CLOCK_REALTIME, &loop_timer);
 
        if (state_timer_reached(&stats, &state_timer)) {
            tetromino_move(&tetromino, &playfield, DOWN);
            stats_tick(&stats);
        } else if ((input = getch()) != ERR) {
            if (convert_input(input) == PAUSE) {
                stats.paused = true;
            } else {
                tetromino_move(&tetromino, &playfield, convert_input(input));
            }
            while ((getch() != ERR)){}
        }

        if (!tetromino_can_move(&tetromino, &playfield, DOWN)) {
            tetromino_freeze(&tetromino, &playfield);
            playfield_clear_lines(&playfield, &stats);
            tetromino_topleft_x = rand() % SPAWN_LIMIT;
            tetromino = tetromino_constructor(next, tetromino_topleft_x, &playfield);
            next = next_tetromino(&plist);
            int spawn_rotations = rand() % SPAWN_LIMIT;
            for (int rotations = 0; rotations < spawn_rotations; rotations++) {
                tetromino_move(&tetromino, &playfield, ROTATE);
            }
        }

        if (stats_can_level_up(&stats)) {
            stats_level_up(&stats, difficulty_level);
        }
    
        if (stats.paused) {
            pause_game(&stats);
        }

        draw_game(&playfield, &stats, &plist, color_mode);

        if (debug) {
            print_debug(&stats, &tetromino, &plist, since_last_frame);
        }

        frame_wait(&loop_timer);
    }
    free(plist.types);
}

int main (int argc, char *argv[]) {

    int difficulty_level = DIFFICULTY_MEDIUM, color_mode = MODE_NO_COLOR, debug = 0;

    char *difficulty_levels[] = {
        "--easy", "--medium", "--hard"
    };
    char *color_modes[] = {
        "--nocolor", "--ascii", "--solid"
    };

    for (int arg = 1; arg < argc; arg++) {
        if (strcmp(argv[arg], difficulty_levels[0]) == 0) {
            difficulty_level = DIFFICULTY_EASY;
        } else if (strcmp(argv[arg], difficulty_levels[2]) == 0) {
            difficulty_level = DIFFICULTY_HARD;
        } else if (strcmp(argv[arg], color_modes[1]) == 0) {
            color_mode = MODE_ASCII_COLOR;
        } else if (strcmp(argv[arg], color_modes[2]) == 0) {
            color_mode = MODE_SOLID_COLOR;
        } else if (strcmp(argv[arg], "--debug") == 0) {
            debug = 1;
        }
    }

    init_curses();
   
    a_game_of_falling_blocks(difficulty_level, color_mode, debug);
    
    game_over();

    uninit_curses();

    return EXIT_SUCCESS;
}

