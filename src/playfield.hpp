#ifndef PLAYFIELD_HPP

#define PLAYFIELD_HPP
#define START_ROW -3
#define START_COL_LIMIT 6
#define PLAYFIELD_HEIGHT 24
#define PLAYFIELD_WIDTH 10

/* A basic collection of standard color pairs, for now. Will extend
 * when the need arises to be more specific.  */
enum { BLACK_BG_BLACK_FG, BLACK_BG_WHITE_FG, BLACK_BG_RED_FG, 
       BLACK_BG_GREEN_FG, BLACK_BG_YELLOW_FG, BLACK_BG_BLUE_FG, 
       BLACK_BG_CYAN_FG, BLACK_BG_MAGENTA_FG }; //ColorPairs;

class Coord;
class Playfield;

#endif

