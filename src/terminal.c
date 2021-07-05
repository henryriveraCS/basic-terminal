#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

//used to read out whether a file is a direcotry or entry
struct stat filestat;

//used to break the loop
bool loadCmd = true;

//used to keep track of the current dir that the user is in
int MAX_BUF = 200;
char *currentDir;

//used to verify if move is legal or not
enum legalCmds{cd=1, ls=2, touch=3, end=0};

//use these to change the font color
void setFontWhite() { printf("\033[0m"); }
void setFontMagenta(){ printf("\033[35m"); }
void setFontBlue() { printf("\033[0;34m"); }
void setFontGreen() { printf("\033[32m"); }
void setFontRed() { printf("\033[31m"); }


//used to get the directory as long as you provide the full path
char *getCurrentDir()
{
	//use buffer to get the path
	char *BUF = getcwd(NULL, 0);
	if(BUF == NULL)
	{
		setFontRed();
		printf("ERROR GETTING DIRECTORY: %s \n", BUF);
		setFontWhite();
	}
	currentDir = BUF;
	return BUF;
}

//florida-style VIM editor
void launchVIM()
{
	printf("VIM STARTED\n");
}

void clearIO()
{
	system("clear");
}

//called to change directory
void changeDir(char *dir)
{
	chdir(dir);
}

//reading files in the current dir
void readDir(char dir)
{
	DIR *folder;
	struct dirent *entry;
	int files = 0;
	
	folder = opendir(".");
	if(folder == NULL)
	{
		printf("UNABLE TO OPEN DIRECTORY");
	}
	
	while( (entry=readdir(folder)) )
	{
		files ++;
		stat(entry->d_name, &filestat);
		//check if this is a dir
		if( S_ISDIR(filestat.st_mode) )
		{
			printf("DIRECTORY: ");
			printf("%s\n", entry->d_name);
		}
		else
		{
			setFontGreen();
			printf("FILE: ");
			setFontWhite();
			printf("%s\n", entry->d_name);
		}
	}
	closedir(folder);
}

void printHelp()
{
	char *cmds = "THE FOLLOWING COMMANDS ARE AVAILABLE ON THIS TERMINAL:\n"
				 "ls\ncd\ntouch\npwd\nclear\nvim\nend OR exit";
	printf("%s", cmds);
}

void readFile(char* cmd)
{
	FILE *file;
	char *cwd = getCurrentDir();
	char c;
	file = fopen((cwd, "/", cmd), "r");
	//while you can still read characters in the file(not end of file)
	while( (c = getc(file)) != EOF)
	{
		printf("%c", c);
	}
	printf("\n");
}

void createFile(char* cmd)
{
	FILE *file;
	char *cwd  = getCurrentDir();
	file = fopen((cwd,"/", cmd), "w");
	fclose(file);

	setFontGreen();
	printf("CREATED FILE: %s/%s\n", cwd, cmd);
	setFontWhite();
}

//this will be used by main to wait for commands
const int awaitCmd()
{
	//initialize the variables to be used later
	char cmd[100];
	//has either the directory path OR file name based off cmd
	char cmd1[100];

	scanf("%s", cmd);
	//if value returned is 0 -> it will execute
	if(strcmp(cmd, "clear") == 0)
	{
		clearIO();	
	}
	else if(strcmp(cmd, "pwd") == 0)
	{
		printf("CURRENT DIRECTORY: %s\n", getCurrentDir());
	}
	else if(strcmp(cmd, "ls") == 0)
	{
		char dir = *getCurrentDir();
		readDir(dir);
	}
	else if(strcmp(cmd, "cd") == 0)
	{
		scanf("%s", cmd1);
		changeDir(cmd1);	
	}
	else if(strcmp(cmd, "touch") == 0)
	{
		scanf("%s", cmd1);
		createFile(cmd1);
	}
	else if(strcmp(cmd, "end") == 0 || strcmp(cmd, "exit") == 0)
	{
		loadCmd = false;	
	}
	else if(strcmp(cmd, "help") == 0)
	{
		printHelp();
	}
	else if(strcmp(cmd, "cat") == 0)
	{
		scanf("%s", cmd1);
		readFile(cmd1);
	}
	else
	{
		printf("Please enter a valid command (help for more info)\n");
	}
}

//used when the program is first initialized
void init()
{
	clearIO();
	char *introMsg = "BASIC TERMINAL INITLIAZED.\n""TYPE 'help' or 'h' for more info.\n";
	printf("%s", introMsg);
}

//used to start the terminal itself
void initTerminal(char *command)
{
	printf("INIT TERMINAL RECEIVED: %s \n", command);
}


int main ()
{
	init();
	while(loadCmd)
	{
		printf("BASIC TERMINAL ");
		setFontBlue();
		printf("[%s]", getCurrentDir());
		setFontWhite();
		char cmd = awaitCmd();
	}
	clearIO();
}
