CC := g++
VPATH = src/
PROG := blocks
COMPILE_FLAGS := -O2 -Iinclude -c -Wall -Wextra
LIBS := -lncurses
	
$(PROG) : main.o Block.o Cell.o Coord.o FallingBlocks.o \
          Playfield.o Tetromino.o TetrominoGenerator.o
	$(CC) -O2 -o $(PROG) $^ $(LIBS); make cleanup

%.o : %.cpp  
	$(CC) $(COMPILE_FLAGS) $^ -o $@

.PHONY: cleanup

cleanup:
	rm -rf *.o
