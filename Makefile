CC := g++
PROG := blocks
BUILDFLAGS := -O2
DEBUGFLAGS := -g -Wall -Wextra
LDFLAGS :=
LIBS := -lncurses
	
$(PROG): src/main.cpp
	$(CC) $(BUILDFLAGS) -o $(PROG) $< $(LDFLAGS) $(LIBS)

debug: src/main.cpp
	$(CC) $(DEBUGFLAGS) -o $(PROG) $< $(LDFLAGS) $(LIBS)

.PHONY: clean, dev

dev:
	clear; make clean; make debug

clean:
	rm -rf $(PROG) src/*.o
