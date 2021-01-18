# FALLING BLOCKS 
##### by sgibber2018

#### w/ default color mode:
![Gameplay Image](images/ascii.png)

#### w/ `--solid` color mode:
![Gameplay Image](images/solid.png)

**Version:** 0.0.4

**Description:** This is a clone of the well-known game of falling puzzle blocks. It is written in C++ and relies on the ncurses library. It runs the whole game in the terminal when the user runs the executable and then resumes business-as-usual when the game is over. **'a', 's', 'd'** keys to move left, down, and right. **'f'** key to rotate the tetromino. **'p'** to pause the game, and **'Q'** to quit.  

**Options:** By default the game uses the "ASCII" look for displaying the tetrominos, which fits the terminal aesthetic. If you prefer a more arcade-style look then using the `--solid` option will toggle it. The default game mode starts at level 1 with a slow drop-rate, but players who want a challenge can use the `--level <number>` option to start at a higher level with a faster drop-rate. The drop-rate maxes out around level 13 (for now), so entering numbers higher than that serves no real purpose. Your score starts at 0 regardless, but I may add a multiplier based on starting level so that getting a high score starting with a faster drop rate is more meaningful. To suppress the intro blurb, use the `--quiet` option. To view the help/about info, use the `--help` option. If your system does not allow for color display in the terminal then it will not use color.

**State of the Project:** It is stable, playable from start to finish, includes various options, and has a lot of room for improvement. After writing the C version I was unsure if I wanted to use Rust or C++ going forward. I decided on C++ because it is both more familiar to me and allows me to practice for some more ambitious projects that I have planned in the near future, for which I will be using C++. I may implement a Rust version alongside this one in the future. I made many improvements to my original C version while converting it to C++: I adopted an object-oriented style, I reduced hard-coding wherever possible, and I have tried to make something here which can potentially be used for other games that have similar requirements. As always, anyone who wishes to contribute or use my code for their own purposes is more than welcome to do so.

**Installation/Dependencies:** (Linux only for now, although I suppose it would work on a BSD-system similarly).
1. Make sure you have basic build tools installed. This varies from distribution to distribution but you will want `gcc`, `make`, and `libncurses-dev`. `gcc` and `make` tend to come packaged together, and sometimes `ncurses` development libraries come with them. For example, on Debian-based systems you will want `build-essential` and `libncurses-dev`. On Arch you will want `base-devel`. Each major Linux distribution should have packages with these dependencies available.
2. Download or clone this repository. 
3. While in the directory of the downloaded or cloned repository, open the terminal and type `make`. 
4. You should now have an executable file named `blocks` which can be run with the command: `./blocks [OPTIONS]`.
- Down the road I may consider offering pre-built binaries, but for now there's some DIY required. If you've never done this sort of thing before then I encourage you to try. If you have any questions then feel free to send me an email using my contact information or look me up on IRC (where I am often present). 

**Disclaimer:** I am not a professional programmer; I am an enthusiastic amateur with a lot to learn. Compile and run at your own risk. 

**Valgrind:** Note that it is normal for a program using `ncurses` to still show some memory in use after exit, if checked with valgrind. This is because of the terminal itself and the way that it is handled. No manual memory allocation is actually done in this program, currently, although that may change in the future with the addition of new game modes or if I increase the scale of the game.

**Resources:** Some resources were helpful while making this program so far:
- *C Programming, a Modern Approach (2nd Edition)* by K.N. King. I surely am not doing this fine book justice by mentioning it in the same breath as my naive and unoptimized C prototype, but it is a wonderful book and was extremely helpful.
- Wikipedia has a good page on tetrominos.
- The ncurses documentation
- I got some great advice from the friendly people at Freenode IRC's ##programming, ##c, and ##c++ channels.
- cppreference.com was invaluable while making the C++ version.
- *C++ Fundamentals* by Antonio Mallia and Francesco Zoffoli was a helpful refresher for some parts.

