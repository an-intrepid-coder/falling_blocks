#include "stats.h"

Stats stats_constructor () {

    Stats new_stats;

    new_stats.score = 0;
    new_stats.ticks = 0;
    new_stats.level = 1;
    new_stats.step_interval = DEFAULT_STEP_INTERVAL_IN_SECONDS;

    return new_stats;
}

bool stats_can_level_up (Stats *stats) {
    bool return_value = false;
    if (stats->score >= stats->level * 100) {
        return_value = true;
    }
    return return_value;
}

void stats_level_up (Stats *stats, int difficulty_level) {
    stats->level += 1;
    switch (difficulty_level) {
        case DIFFICULTY_EASY:
            stats->step_interval *= STEP_INTERVAL_MULTIPLIER_SLOW;
        break;
        case DIFFICULTY_MEDIUM:
            stats->step_interval *= STEP_INTERVAL_MULTIPLIER_MEDIUM;
        break;
        case DIFFICULTY_HARD:
            stats->step_interval *= STEP_INTERVAL_MULTIPLIER_FAST;
        break;
    }
}

void stats_tick (Stats *stats) {
    stats->ticks += 1;
}

void stats_score_up (Stats *stats, int score_up_amount) {
    stats->score += score_up_amount;
}
