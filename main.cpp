#include <iostream>
#include "FallingBlocks.hpp"

using std::cout;
using std::cin;
using std::endl;

int main()  // CLI args coming up
{
    const std::string version = "0.0.1";

    {
        FallingBlocks game = FallingBlocks();
        game.game_loop();
    }

    cout << "Thank you for playing falling blocks version " << version << endl;
    cout << "Play again soon!" << endl;
}
