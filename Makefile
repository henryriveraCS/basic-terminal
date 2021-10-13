#use gcc compiler
CC = gcc

#display any warnings/errors
CFLAGS += -Wall -I.

#external libraries
EXT_LIBS = -lcurl

#what objects are part of final program
OBJ = src/terminal.c src/fileIO.c src/floridaVim.c src/directoryIO.c src/config.c src/netIO.c

#final program
TARGET= basic-terminal

#build B-terminal
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(EXT_LIBS)

#rebuild out the program
clean:
	rm $(TARGET)

#allows you to rebuild the project in one line
new: clean
	$(MAKE)
