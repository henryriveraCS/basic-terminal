#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "floridaVim.h"
#include "fileIO.h"

//used to break the loop
bool loadCmd = true;

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
	else if(strcmp(cmd, "vim") == 0)
	{
		scanf("%s", cmd1);
		launchVim(cmd1);
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
		char msg[] = "Please enter a valid command (help for more info)";
		errorMsg(msg);
	}
}

//used when the program is first initialized
void init()
{
	clearIO();
	char *introMsg = "BASIC TERMINAL INITLIAZED.\n""TYPE 'help' or 'h' for more info.\n";
	printf("%s", introMsg);
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
		printf(">");
		char cmd = awaitCmd();
	}
	clearIO();
}
