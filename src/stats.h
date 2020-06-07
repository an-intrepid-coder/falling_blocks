#ifndef STATS_H
#define STATS_H

#define CLEAR_ROW_SCORE 10
#define DEFAULT_STEP_INTERVAL_IN_SECONDS (double) 1
#define STEP_INTERVAL_MULTIPLIER_SLOW (double) .90
#define STEP_INTERVAL_MULTIPLIER_MEDIUM (double) .80
#define STEP_INTERVAL_MULTIPLIER_FAST (double) .65
#define STEP_INTERVAL_MAX (double) .112

typedef enum {DIFFICULTY_EASY, DIFFICULTY_MEDIUM, DIFFICULTY_HARD} Difficulty_Levels;

typedef struct {
    /* A struct to keep track of game statistics. Passes some to the hud display.  */
    int score;
    int ticks;
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
