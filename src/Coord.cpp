#include "Coord.hpp"

Coord::Coord(int y, int x) : y(y), x(x) {}
Coord::Coord(Coord origin, struct Delta delta) : y(origin.get_y() + delta.dy), x(origin.get_x() + delta.dx) {}

bool Coord::in_bounds(int rows, int cols) { return y < rows && x >= 0 && x < cols; }
