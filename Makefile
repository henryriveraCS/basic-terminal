#use gcc compiler
CC = gcc

#-I. will look at current directory (./BASIC-TERMINAL)
CFLAGS = -Wall -I.

#dependancies we need
DEPS = src/fileIO.h src/floridaVim.h

#what objects are part of final program
OBJ = src/terminal.c 

#final program
TARGET= basic-terminal

#build B-terminal
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(DEPS) $(OBJ)

#clean out the program itself to recompile later if needed
clean:
	rm -f $(TARGET) *~
