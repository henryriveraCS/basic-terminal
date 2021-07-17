#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
//#include <errno.h>

//create buffer -> output buffer
#include "./include/fileIO.h"
#include "./include/directoryIO.h"
#include "./include/config.h"
#include "./include/floridaVim.h"

/*
	a control character is separated from it's printable version by decimal 96, or 110 0000 binary

	The definition below does a bitwise AND between (k) and 0x1f
	we define (ctrl + (q)) bits as our exit method for the program

	7 BIT ASCII & math                    7 bit HEX & math
 	q 	 = 111 0001                     	q = 71
	&    = 001 1111                         0x1f
	---------------                       ----------------

 ctrl-q  = 001 0001  

	if you press CTRL + a key together the 6th and 7th bit get removed in nix systems

 	therefore you have: 1 0001 
	0x1f will take the last 5 bits (1 0001) in this case 
	end result is: 10001 -> which converts to 017 -> which is the ASCII representation for CTRL (meaning ctrl + q was pressed)
*/

//perform logical AND between k AND 0x1f

#define CTRL_KEY(k) ((k) & 0x1f)

//determines if user is still editing or ended the editor process
int editing = true;
//original terminal
struct termios orig_termios;

//used to change wasd into arrow keys
enum editorKey 
{
	ARROW_LEFT = 'a',
	ARROW_RIGHT = 'd',
	ARROW_UP = 'w',
	ARROW_DOWN = 's'
};

void died(const char *s)
{
	perror(s);
	exit(1);
}

char *editorRowsToString(int *wordBufLen)
{
	int totlen = 0;
	//int j;
	
	char *buf = malloc(totlen);

	return buf;
}

void disableRawMode(void)
{
	if( tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
	{
		died("tcsetattr");
	}
}

void enableRawMode(void)
{
	//get original attributes of terminal and storing it to original termios
	if( tcgetattr(STDIN_FILENO, &orig_termios) == -1 )
	{
		died("tcgetattr");
	}
	//from stlid -> calls function once program ends
  	//atexit(disableRawMode);

	//instace representing the text editor
	struct termios raw = orig_termios;
	//disabling IXON (i = input flag, XON = control names that CTRL+S/CTRL+Q produce)
	//ICRNL REPRESENTS CTRL+M FIX
	raw.c_iflag &= ~(IXON | ICRNL);
	raw.c_oflag &= ~(OPOST);
	//performing bitwise AND on ECHO to turn it off | DISABLING CANON MODE | DISABLING CTRL-C + CTRL+Z
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	//setting attribute of editor
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


int launchFloridaVim(char *dir, char *fileName)
{
	//FILE *tmpFile;
	int maxFileSize = 2000;
	char *filePtr = (char *) malloc(UNIX_MAX_PATH);
	char *tmpFilePath = (char *) malloc(UNIX_MAX_PATH);
	char *tmpFileName = (char *) malloc(UNIX_MAX_PATH);
	char wordBuf[maxFileSize]; //(char *) malloc(maxFileSize);
	strncpy(wordBuf, "", maxFileSize);

	buildPath(filePtr, dir, fileName);

	if(filePtr == NULL)
	{
		char *msg = "ERROR OPENING FILE. PLEASE MAKE SURE IT EXISTS OR USE TOUCH [FILE_NAME] TO USE USE IT";
		errorMsg(msg);
		return -1;
	}

	//make sure it's a file that exists, otherwise error out
	if(isFile(filePtr) == false)
	{
		char *msg = "COULD NOT FIND FILE, PLEASE MAKE SURE IT EXISTS";
		errorMsg(msg);
		return -1;
	}
	//create tmp file
	strncpy(tmpFileName, fileName, UNIX_MAX_PATH);
	strncat(tmpFileName, ".tmp", UNIX_MAX_PATH);
	//copyFile(dir, fileName, tmpFileName);

	//after copying new file -> get full path of this file to open it and edit
	strncpy(tmpFilePath, dir, UNIX_MAX_PATH);
	strncat(tmpFilePath, "/", UNIX_MAX_PATH);
	strncat(tmpFilePath, fileName, UNIX_MAX_PATH);
	strncat(tmpFilePath, ".tmp", UNIX_MAX_PATH);

	//VIM RELATED calls HERE

	//print out file name at the top
	printf("%s\n", tmpFileName);
	//enable raw mode for termios
	enableRawMode();

	while(editing)
	{
		char c = '\0';
		if ( read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) died("read");
		{
			died("read");	
		}
		if( iscntrl(c) )
		{
			printf("%d\r\n", c);
		}
		else
		{
			printf("%d ('%c')\r\n", c, c);
		}

		if ( c == CTRL_KEY('q') )
		{
			break;
		}
	}

	//return the original settings that basic-terminal had before entering editor mode
	disableRawMode();
	return 0;
}
