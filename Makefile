#use gcc compiler
CC = gcc

#-I. will look at current directory (./BASIC-TERMINAL)
CFLAGS = -Wall -I.

#dependancies we need
DEPS = include/fileIO.h include/floridaVim.h

#what objects are part of final program
OBJ = src/terminal.c 

#final program
TARGET= basic-terminal

#build B-terminal
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(DEPS) $(OBJ)

#rebuild out the program
clean:
	rm $(TARGET)

#allows you to rebuild the project in one line
new: clean
	$(MAKE)
