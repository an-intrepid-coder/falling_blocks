CC := g++
PROG := blocks
COMPILE_FLAGS := -O2 -Iinclude -c -Wall -Wextra
LIBS := -lncurses
	
$(PROG): main.o Block.o Cell.o Coord.o FallingBlocks.o Playfield.o \
	 Tetromino.o TetrominoGenerator.o
	$(CC) -O2 -o $(PROG) $^ $(LIBS); make cleanup

main.o: main.cpp
	$(CC) $(COMPILE_FLAGS) $^ 

Block.o: src/Block.cpp
	$(CC) $(COMPILE_FLAGS) $^ 

Cell.o: src/Cell.cpp
	$(CC) $(COMPILE_FLAGS) $^ 

Coord.o: src/Coord.cpp
	$(CC) $(COMPILE_FLAGS) $^ 

FallingBlocks.o: src/FallingBlocks.cpp
	$(CC) $(COMPILE_FLAGS) $^ 

Playfield.o: src/Playfield.cpp
	$(CC) $(COMPILE_FLAGS) $^ 

Tetromino.o: src/Tetromino.cpp
	$(CC) $(COMPILE_FLAGS) $^ 

TetrominoGenerator.o: src/TetrominoGenerator.cpp
	$(CC) $(COMPILE_FLAGS) $^ 

.PHONY: cleanup

cleanup:
	rm -rf *.o
