#ifndef COORD_H
#define COORD_H

typedef struct {
    /* An object representing a pair of (y, x) coordinates.  */
    int y;
    int x;
} Coord;

Coord coord_constructor (int y, int x);

#endif
