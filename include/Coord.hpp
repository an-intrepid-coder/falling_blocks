#ifndef COORD_HPP
#define COORD_HPP

struct Delta { int dy, dx; };

class Coord
{
    public:
        Coord(int y, int x);
        Coord(Coord origin, struct Delta delta);

        int get_y() { return y; }
        int get_x() { return x; }
        bool in_bounds(int rows, int cols);

    private:
        int y;
        int x;
};

#endif // COORD_HPP
