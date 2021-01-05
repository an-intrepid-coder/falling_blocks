#include "tetromino.hpp"

/* This is the most important part for improving over my previous
 * version. I have a few options when it comes to rotation and collision
 * detection. The way I did it before is not ideal. I will do something
 * better this time. I think that, by far, the best way to do this is
 * to treat them like sprites. That is, a rectangular object where some
 * pixels are "transparent" and "non-colliding". This would be 2x3 for 6
 * tetrominos, and 2x4 for one of them. Then I could rotate them and all of that
 * much more easily, and probably even use standard math for things like matrix
 * rotation or whatever the technical term is. That is by far the best way to do
 * it!  */
class Tetromino {
    public:



    private:
        int type;
        int configuration;
        
};

