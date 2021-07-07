#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

//MACROS
//max path length allowed by win32 api
#define WIN_MAX_PATH 260
//max unix path just for consistency
#define UNIX_MAX_PATH 520

//font color macros
#define setFontWhite { printf("\033[0m"); }
#define setFontMagenta { printf("\033[35m"); }
#define setFontBlue { printf("\033[0;34m"); }
#define setFontGreen { printf("\033[32m"); }
#define setFontRed { printf("\033[31m"); }

//used to read out whether a file is a direcotry or entry
struct stat filestat;
char* cwd;

//used to get the directory as long as you provide the full path
char *getCurrentDir()
{
    //use buffer to get the path
    char *BUF = getcwd(NULL, 0);
    if(BUF == NULL)
    {
        setFontRed;
        printf("ERROR GETTING DIRECTORY: %s \n", BUF);
        setFontWhite;
    }
	cwd = BUF;
    return BUF;
}

void errorMsg(char *error)
{
	setFontRed;
	printf("\nWARNING: ");
	setFontWhite;
	printf("%s\n", error);
}   

void printHelp()
{
    char *cmds = "THE FOLLOWING COMMANDS ARE AVAILABLE ON THIS TERMINAL:\n"
                 "ls\ncd\ntouch\npwd\nclear\nvim\nend OR exit\n";
    printf("%s", cmds);
}


//reading files in the current dir
void readDir()
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
            setFontMagenta;
            printf("DIRECTORY: ");
            setFontWhite;
            printf("%s\n", entry->d_name);
        }
        else
        {
            setFontGreen;
            printf("FILE: ");
            setFontWhite;
            printf("%s\n", entry->d_name);
        }
    }
    closedir(folder);
}

void readFile(const char* cwd, const char* cmd)
{
    FILE *file;
    char c;
	//260 char length is the MAX_PATH of windows32 API library
	char tmp[UNIX_MAX_PATH] = { 0 };
	strncat(tmp, cwd, (UNIX_MAX_PATH - strlen(cwd) - 1));
	strncat(tmp, "/", (UNIX_MAX_PATH - strlen("/") - 1));
	strncat(tmp, cmd, (UNIX_MAX_PATH - strlen(cmd) - 1));

	//verify that the array didn't overflow, if it did -> return error
	if(strnlen(tmp, UNIX_MAX_PATH) < UNIX_MAX_PATH)
	{
		//make sure command wasn't empty
		if(cmd == NULL)
		{
			char message[] = "FILE NOT FOUND, PLEASE MAKE SURE IT EXISTS";
			errorMsg(message);
		}
		else
		{
			file = fopen(tmp, "r");
			//while you can still read characters in the file(not end of file)
			while( (c = getc(file)) != EOF)
			{
				printf("%c", c);
			}
			printf("\n");
		}
	}
	else
	{
		char *msg = "PATH DIRECTORY IS TOO LONG";
		errorMsg(msg);
	}
}


void clearIO()
{
    system("clear");
}

void createFile(const char* cwd, const char* cmd)
{
    FILE *file;
	char tmp[UNIX_MAX_PATH] = { 0 };
	printf("\nCMD: %s", cmd);
	printf("\nCWD: %s", cwd);
	strncat(tmp, cwd, (UNIX_MAX_PATH - strlen(cwd) -1));
	strncat(tmp, "/", (UNIX_MAX_PATH - strlen("/") -1));
	strncat(tmp, cmd, (UNIX_MAX_PATH - strlen(cmd) - 1));
	printf("\nHERE: %s\n", tmp);
	//verify the path won't overflow

	if(strnlen(tmp, UNIX_MAX_PATH) < UNIX_MAX_PATH)
	{
    	file = fopen(tmp, "w");
    	fclose(file);

		setFontGreen;
		printf("CREATED FILE: %s/%s\n", cwd, cmd);
		setFontWhite;
	}
	else
	{
		char *msg = "OVERFLOW ON FILE CREATION";
		errorMsg(msg);
	}
}


//called to change directory
void changeDir(char *dir)
{
    chdir(dir);
}
