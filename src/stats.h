#ifndef STATS_H
#define STATS_H

#define CLEAR_ROW_SCORE 10
#define DEFAULT_STEP_INTERVAL_IN_SECONDS (double) 1

typedef struct {
    /* A struct to keep track of game statistics. Passes some to the hud display.  */
    long long int score;
    long long int ticks;
    int level;
    double step_interval;
} Stats;

Stats stats_constructor ();
bool stats_can_level_up (Stats *stats);
void stats_level_up (Stats *stats);
void stats_tick (Stats *stats);
void stats_score_up (Stats *stats, int score_up_amount);

#endif
