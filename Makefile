CC := gcc
PROG := blocks
BUILDFLAGS := -O2
DEBUGFLAGS := -g -Wall -Wextra
LDFLAGS :=
LIBS := -lncurses
	
$(PROG): src/falling_blocks.c
	$(CC) $(BUILDFLAGS) -o $(PROG) $< $(LDFLAGS) $(LIBS)

debug: src/falling_blocks.c
	$(CC) $(DEBUGFLAGS) -o $(PROG) $< $(LDFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -rf $(PROG) src/*.o
