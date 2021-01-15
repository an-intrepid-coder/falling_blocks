#include <iostream>
#include "FallingBlocks.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::atoi;
using std::exit;

const string version = "0.0.1";

struct Flags { int starting_level; bool solid, help, quiet; };

struct Flags get_flags(int argc, char* argsv[])
{
    struct Flags flags{1, false, false, false};

    string level = "--level", solid = "--solid", quiet = "--quiet", help = "--help";
    for (int arg = 1; arg < argc; arg++)
    {
        if (argsv[arg] == level && argc > arg + 1)
        {
            int get_level = atoi(argsv[arg + 1]);
            if (get_level > 1)
                flags.starting_level = get_level;
        }
        else if (argsv[arg] == solid)
        {
            flags.solid = true;
        }
        else if (argsv[arg] == quiet)
        {
            flags.quiet = true;
        }
        else if (argsv[arg] == help)
        {
            flags.help = true;
        }
    }

    return flags;
};

void print_help_and_exit()
{
    cout << "\nFalling Blocks version " << version << " by sgibber2018"
         << "\n\n\tFit the falling puzzle blocks together to clear lines before"
         << "\n\tthey reach the top of the screen!"
         << "\n\nOptions:"
         << "\n--level <number>\tChoose starting level for accelerated difficulty."
         << "\n\t\t\tEach level speeds up drop-rate until it hits max speed."
         << "\n\t\t\tCurrently it will hit max speed around level 13."
         << "\n\n--solid\t\t\tUses an arcade-y 'solid' look for the blocks."
         << "\n\t\t\tBy default an 'ASCII' look is used."
         << "\n\n--quiet\t\t\tSuppresses the intro message."
         << "\n\n--help\t\t\tPrints information about options to the display." << endl;
    exit(0);
}

void print_intro()
{
    cout << "\nWelcome to Falling Blocks version " << version
         << "\n\nKeys: LEFT: a/A | DOWN: s/S | RIGHT: d/D | ROTATE: f/F | QUIT: Q"
         << "\n\nENTER to begin! Good luck!" << endl;
    getchar();
}

void print_outro(int score)
{
    cout << "\nYou finished with a score of: " << score << "!"
         << "\nPlay again soon!\n" << endl;
}

int main(int argc, char* argsv[])
{
    Flags flags = get_flags(argc, argsv);

    if (flags.help)
        print_help_and_exit();

    if (!flags.quiet)
        print_intro();

    unsigned long int score;
    {
        FallingBlocks game = FallingBlocks(flags.starting_level, flags.solid);
        score = game.game_loop();
    }

    print_outro(score);

    return 0;
}
