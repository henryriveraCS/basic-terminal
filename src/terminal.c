#include "./include/floridaVim.h"
#include "./include/fileIO.h"
#include "./include/directoryIO.h"
#include "./include/netIO.h"
#include "./include/config.h"

#include <string.h>

bool loadCmd = true;
//used to keep "history" of commands issued
int historyLen = 4;
char *history[5] = { NULL };
int historyCount = 0;

void showHistory()
{
	printf("SHOWING HISTORY \n");
	for(int i=0; i < historyLen; i++)
	{
		printf("%s", history[historyLen]);
	}
}

//manually check for all commands (and append as into history as needed
void updateHistory(char *cmd, char *cmd1, char *cmd2, char* cmd3)
{
	char log[UNIX_MAX_PATH];
	int totalLen = (int)strlen(cmd);
	//char log1[UNIX_MAX_PATH];
	//char log2[UNIX_MAX_PATH];
	//char log3[UNIX_MAX_PATH];
	if(cmd && (cmd[0] != 0))
	{
		//strncpy(log, cmd, strlen(cmd));
		//printf("CMD FILLED\n");
		strncpy(log, cmd, UNIX_MAX_PATH-totalLen);
	}
	if(!cmd1 && (cmd[0] != 0))
	{
		printf("CMD1 FILLED\n");
		//strncpy(log, " ", 1);
		//strncpy(log, cmd1, strlen(cmd1));
		strncpy(log, cmd1+totalLen, UNIX_MAX_PATH-(int)strlen(cmd1));
		totalLen += (int)strlen(cmd1);
	}
	if(!cmd2 && (cmd2[0] != 0))
	{
		//strncpy(log, " ", 1);
		printf("CMD2 FILLED\n");
		strncpy(log, cmd2, totalLen);
		totalLen += (int)strlen(cmd2);
	}
	if(!cmd3 && (cmd3[0] != 0))
	{
		//strncpy(log, " ", 1);
		printf("CMD3 FILLED\n");
		strncpy(log, cmd3, totalLen);
		totalLen += (int)strlen(cmd3);
	}
	//history[historyCount] = log;
	//USE THIS
	//strncpy(log, cmd, strlen(cmd));
	//printf("ADDING HISTORY TEST: %.*s\n", (int)strlen(cmd), log);
	
	//printf("passed log: %.*s\n", totalLen, log);
	history[historyCount] = cmd;
	historyCount += 1;
	//check if the log is full, if it is -> reset to 0
	if(historyCount > historyLen+1)
	{
		historyCount = 0;
	}
}

//this will be used by main to wait for commands
bool awaitCmd()
{
	//initialize the variables to be used later
	char cmd[UNIX_MAX_PATH];
	char cmd1[UNIX_MAX_PATH];

	//used for cmds that need 2+ args (E.G: cp originalFile newFile || rm -d directoryName)
	char cmd2[UNIX_MAX_PATH];
	char cmd3[UNIX_MAX_PATH];

	//initialize values to each command so we can properly store/check if there are values
	//later
	cmd[0] = 0;
	cmd[1] = 0;
	cmd[2] = 0;
	cmd[3] = 0;

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
		showHistory();
	}
	else
	{
		char msg[] = "Please enter a valid command (help for more info)";
		errorMsg(msg);
	}
	//right before loop -> update history with all values
	updateHistory(cmd, cmd1, cmd2, cmd3);
	return true;
}

//used when the program is first initialized
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
