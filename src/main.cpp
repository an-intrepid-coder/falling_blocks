#include <iostream>
#include "FallingBlocks.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::atoi;
using std::exit;
using std::min;

const string version = "0.1.2";

struct Flags { int starting_level; bool solid, help, quiet, animate; };

struct Flags get_flags(int argc, char* argsv[])
{
    struct Flags flags{1, true, false, false, true};

    string level = "--level", ascii = "--ascii", quiet = "--quiet", help = "--help", animate = "--noanimate";
    for (int arg = 1; arg < argc; arg++)
    {
        if (argsv[arg] == level && argc > arg + 1)
        {
            int get_level = atoi(argsv[arg + 1]);
            if (get_level > 1)
                flags.starting_level = get_level;
        }
        else if (argsv[arg] == ascii)
        {
            flags.solid = false;
        }
        else if (argsv[arg] == quiet)
        {
            flags.quiet = true;
        }
        else if (argsv[arg] == help)
        {
            flags.help = true;
        }
        else if (argsv[arg] == animate)
        {
            flags.animate = false;
        }
    }

    return flags;
};

void print_help_and_exit()
{
    cout << "\nFalling Blocks version " << version << " by sgibber2018"
         << "\n\nDescription:\t\tFit the falling puzzle blocks together to clear lines before"
         << "\n\t\t\tthey reach the top of the screen!"
         << "\n\nOptions:"
         << "\n--level <number>\tChoose starting level for accelerated difficulty."
         << "\n\t\t\tEach level speeds up drop-rate until it hits max speed."
         << "\n\t\t\tCurrently it will hit max speed around level 13."
         << "\n\n--ascii\t\t\tUses an 'ASCII' aesthetic instead of solid"
         << "\n\t\t\tblocks of color."
         << "\n\n--quiet\t\t\tSuppresses the intro message."
         << "\n\n--noanimate\t\tSuppresses the level and game over animations."
         << "\n\n--help\t\t\tPrints information about options to the display." << endl;
    exit(0);
}

void print_intro()
{
    cout << "\nWelcome to Falling Blocks version " << version
         << "\n\nKeys: \tLEFT: a/A | DOWN: s/S | RIGHT: d/D"
         << "\n\tROTATE: f/F or j/J | DROP: Spacebar | QUIT: Q"
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
        FallingBlocks game = FallingBlocks(flags.starting_level, flags.solid, flags.animate);
        score = game.game_loop();
    }

    score += min(MAX_LEVEL, flags.starting_level - 1) * LINE_SCORE * LINES_TO_LEVEL;

    print_outro(score);

    return 0;
}

