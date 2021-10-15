#include "./include/floridaVim.h"
#include "./include/fileIO.h"
#include "./include/directoryIO.h"
#include "./include/netIO.h"
#include "./include/config.h"

#include <string.h>
#include <stdlib.h>

//used to keep "history" of commands issued
#define historyLen 10
int historyCount = 0;


//used to keep program running
bool loadCmd = true;
//0 command run is valid (add to history), 1 if it is not
int cmdValid = 0;

void showHistory(char history[historyLen][UNIX_MAX_PATH])
{
	printf("COMMAND HISTORY \n");
	for(int i=0; i < historyLen; i++)
	{
		if(strcmp(history[i], "000") != 0)
		{
			printf("Command %d: %s\n", i, history[i]);
		}
	}
}

void updateHistory(char history[historyLen][UNIX_MAX_PATH], char cmd[], char cmd1[], char cmd2[], char cmd3[])
{
	char log[UNIX_MAX_PATH];
	log[0] = '\0';
	//int totalLen = 0;
	if(cmd[0] != '\0')
	{
		//totalLen += (int)strlen(cmd);
		strncpy(log, cmd, UNIX_MAX_PATH);
	}
	if(cmd1[0] != '\0')
	{
		strncat(log, " ", UNIX_MAX_PATH);
		strncat(log, cmd1, UNIX_MAX_PATH);
	}
	if(cmd2[0] != '\0')
	{
		strncat(log, " ", UNIX_MAX_PATH);
		strncat(log, cmd2, UNIX_MAX_PATH);
	}
	if(cmd3[0] != '\0')
	{
		strncat(log, " ", UNIX_MAX_PATH);
		strncat(log, cmd3, UNIX_MAX_PATH);
	}

	strcpy(history[historyCount], log);
	historyCount += 1;

	//check if the log is full, if it is -> reset to 0
	if(historyCount > historyLen)
	{
		historyCount = 0;
	}
}

void getHistory(char *cmd, char *logID, char history[historyLen][UNIX_MAX_PATH])
{
	int id = atoi(logID);
	char toPut[UNIX_MAX_PATH];
	toPut[0] = '\0';
	strcpy(toPut, history[id]);
	if(toPut[0] != '\0')
	{
		system(toPut);
		printf("EXECUTED: %s\n", toPut);
	}
}

//this will be used by main to wait for commands
bool awaitCmd(char history[historyLen][UNIX_MAX_PATH])
{
	//initialize the variables to be used later
	char cmd[UNIX_MAX_PATH];
	char cmd1[UNIX_MAX_PATH];

	//used for cmds that need 2+ args (E.G: cp originalFile newFile || rm -d directoryName)
	char cmd2[UNIX_MAX_PATH];
	char cmd3[UNIX_MAX_PATH];

	cmd[0] = '\0';
	cmd1[0] = '\0';
	cmd2[0] = '\0';
	cmd3[0] = '\0';

	scanf("%s", cmd);

	if(strcmp(cmd, "get") == 0)
	{
		scanf("%s", cmd1);
		getHistory(cmd, cmd1, history);
	}
	else if(strcmp(cmd, "clear") == 0)
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
	else if(strcmp(cmd, "curl") == 0)
	{
		scanf("%s", cmd1);
		if(strcmp(cmd1, "-d") == 0)
		{
			//this is the URL
			scanf("%s", cmd2);
			//this is the fileName
			scanf("%s", cmd3);
			downloadURL(cmd2, cmd3);
		}
		else
		{
			printURL(cmd1);
		}
	}
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
	else if(strcmp(cmd, "history") == 0)
	{
		showHistory(history);
	}
	else
	{
		cmdValid = 0;
		char msg[] = "Please enter a valid command (help for more info)";
		errorMsg(msg);
	}
	//right before loop -> update history with all values
	//don't add "history" command into history to save space
	if(strcmp(cmd, "history") != 0 && cmdValid == 1)
	{
		updateHistory(history, cmd, cmd1, cmd2, cmd3);
	}
	cmdValid = 1;
	return true;
}

void init()
{
	clearIO();
	char *dir = getCurrentDir();
	//load up the banner from ./banner.txt
	char *fileName = "./banner.txt";
	readFile(dir, fileName);
	char *introMsg = "BASIC TERMINAL INITIALIZED.\nTYPE 'help' for info on more commands.\n";
	printf("%s", introMsg);
}

int main ()
{
	bool loadCmd = true;
	char history[10][UNIX_MAX_PATH];
	//initialize the values once so weird outputs don't show up in history log
	for(int i=0; i < historyLen; i++)
	{
		strcpy(history[i], "000");
	}
	init();
	while(loadCmd)
	{
		printf("BASIC TERMINAL ");
		setFontBlue();
		printf("[%s]", getCurrentDir());
		setFontWhite();
		printf(">");
		loadCmd = awaitCmd(history);
	}
}
