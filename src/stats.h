#ifndef STATS_H
#define STATS_H

#define CLEAR_ROW_SCORE 10

#define DEFAULT_STEP_INTERVAL_NSEC (double) 1.0
#define STEP_INTERVAL_FLOOR (double) .122

#define EASY_JUMP (double) .025
#define MEDIUM_JUMP (double) .05
#define HARD_JUMP (double) .4

typedef enum {DIFFICULTY_EASY, DIFFICULTY_MEDIUM, DIFFICULTY_HARD} Difficulty_Levels;

typedef struct {
    /* A struct to keep track of game statistics. Passes some to the hud display.  */
    long int score;
    long int ticks;
    int level;
    double step_interval;
    bool paused;
} Stats;

Stats stats_constructor ();
bool stats_can_level_up (Stats *stats);
void stats_level_up (Stats *stats, int difficulty_level);
void stats_tick (Stats *stats);
void stats_score_up (Stats *stats, int score_up_amount);

#endif

