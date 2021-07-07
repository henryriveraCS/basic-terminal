#include <stdio.h>

#include "./include/floridaVim.h"
#include "./include/fileIO.h"

bool loadCmd = true;

//this will be used by main to wait for commands
bool awaitCmd()
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
	else if(strcmp(cmd, "cat") == 0)
	{
		scanf("%s", cmd1);
		char *dir = getCurrentDir();
		readFile(dir, cmd1);
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
		readDir();
	}
	else if(strcmp(cmd, "cd") == 0)
	{
		scanf("%s", cmd1);
		changeDir(cmd1);	
	}
	else if(strcmp(cmd, "touch") == 0)
	{
		scanf("%s", cmd1);
		char *dir = getCurrentDir();
		printf("ELIF: \nCWD:%s CMD: %s\n", dir, cmd1);
		createFile(dir, cmd1);
	}
	else if(strcmp(cmd, "end") == 0 || strcmp(cmd, "exit") == 0)
	{
		return false;
	}
	else if(strcmp(cmd, "help") == 0)
	{
		printHelp();
	}
	else
	{
		char msg[] = "Please enter a valid command (help for more info)";
		errorMsg(msg);
	}
	return true;
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
		setFontBlue;
		printf("[%s]", getCurrentDir());
		setFontWhite;
		printf(">");
		loadCmd = awaitCmd();
	}
	clearIO();
}
