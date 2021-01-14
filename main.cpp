#include <iostream>
#include "FallingBlocks.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::atoi;

int get_level_arg(int argc, char* argsv[])
{
    string prelude = "--level";
    for (auto arg = 1; arg < argc; arg++)
    {
        if (argsv[arg] == prelude && argc > arg + 1)
        {
            int level = atoi(argsv[arg + 1]);
            if (level > 0)
                return level;
        }
    }
    return 1;
}

int main(int argc, char* argsv[])
{
    const string version = "0.0.1";

    cout << "\nWelcome to Falling Blocks version " << version << endl;
    cout << "\nKeys: LEFT = 'a' | DOWN = 's' | RIGHT = 'd' | ROTATE = 'f'" << endl;
    cout << "\nENTER to begin! Good luck!" << endl;
    getchar();

    unsigned long int score;
    {
        FallingBlocks game = FallingBlocks(get_level_arg(argc, argsv));
        score = game.game_loop();
    }

    cout << "\nYou finished with a score of: " << score << "!" << endl;
    cout << "Play again soon!\n" << endl;

    return 0;
}
