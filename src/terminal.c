#include "./include/floridaVim.h"
#include "./include/fileIO.h"
#include "./include/directoryIO.h"
#include "./include/netIO.h"
#include "./include/config.h"

#include <string.h>

bool loadCmd = true;

//this will be used by main to wait for commands
bool awaitCmd()
{
	//initialize the variables to be used later
	char cmd[UNIX_MAX_PATH];
	char cmd1[UNIX_MAX_PATH];

	//used for cmds that need 2+ args (E.G: cp originalFile newFile || rm -d directoryName)
	char cmd2[UNIX_MAX_PATH];
	char cmd3[UNIX_MAX_PATH];

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
	else if (strcmp(cmd, "ping") == 0)
	{
		scanf("%s", cmd1);
		int ipLength = strlen(cmd1);
		ping(cmd1, ipLength);
	}
	/*
	else if(strcmp(cmd, "vim") == 0)
	{
		scanf("%s", cmd1);
		char *dir = getCurrentDir();
		launchFloridaVim(dir, cmd1);
	}
	*/
	else if(strcmp(cmd, "mkdir") == 0)
	{
		scanf("%s", cmd1);
		char *dir = getCurrentDir();
		createDir(dir, cmd1);
	}
	else if(strcmp(cmd, "rm") == 0)
	{
		scanf("%s", cmd1);
		//check if cmd1 is rm -d (which will instead delete a directory
		if(strcmp(cmd1, "-d") == 0)
		{
			scanf("%s", cmd2);
			char *dir = getCurrentDir();
			removeDir(dir, cmd2);
		}
		else
		{
			char *dir = getCurrentDir();
			removeFile(dir, cmd1);
		}
	}
	else if(strcmp(cmd, "cp") == 0)
	{
		scanf("%s", cmd1);
		//if user wants to copy a dir
		if(strcmp(cmd1, "-d") == 0)
		{
			scanf("%s", cmd2);
			scanf("%s", cmd3);
			char *dir = getCurrentDir();
			copyDir(dir, cmd2, cmd3, false);
		}
		else
		{
			scanf("%s", cmd2);
			char *dir = getCurrentDir();
			copyFile(dir, cmd1, cmd2);	
		}
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
		createFile(dir, cmd1);
	}
	else if(strcmp(cmd, "exit") == 0)
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
	//load up the banner from ./banner.txt
	clearIO();
	char *dir = getCurrentDir();
	char *fileName = "./banner.txt";
	readFile(dir, fileName);
	char *introMsg = "BASIC TERMINAL INITIALIZED.\nTYPE 'help' for info on more commands.\n";
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
		loadCmd = awaitCmd();
	}
}
