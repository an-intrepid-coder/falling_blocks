#include "stats.h"

Stats stats_constructor () {

    Stats new_stats = {
        .score = 0,
        .ticks = 0,
        .level = 1,
        .step_interval = DEFAULT_STEP_INTERVAL_NSEC,
        .paused = false,
    };

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
    double result = stats->step_interval;
    switch (difficulty_level) {
        case DIFFICULTY_EASY:
            result -= EASY_JUMP;
        break;
        case DIFFICULTY_MEDIUM:
            result -= MEDIUM_JUMP;
        break;
        case DIFFICULTY_HARD:
            result -= HARD_JUMP;
        break;
    }
    if (result >= STEP_INTERVAL_FLOOR) {
        stats->step_interval = result;
    } else {
        stats->step_interval = STEP_INTERVAL_FLOOR;
    }
}

void stats_tick (Stats *stats) {
    stats->ticks += 1;
}

void stats_score_up (Stats *stats, int score_up_amount) {
    stats->score += score_up_amount;
}

