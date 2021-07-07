#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

//MACROS
//max path length allowed by win32 api
#define MAX_PATH 260

//font color macros
#define setFontWhite { printf("\033[0m"); }
#define setFontMagenta { printf("\033[35m"); }
#define setFontBlue { printf("\033[0;34m"); }
#define setFontGreen { printf("\033[32m"); }
#define setFontRed { printf("\033[31m"); }

//used to read out whether a file is a direcotry or entry
struct stat filestat;

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
	char tmp[260];
	strncat(tmp, cwd, MAX_PATH-1);
	strncat(tmp, "/", MAX_PATH-1);
	strncat(tmp, cmd, MAX_PATH-1);

	//verify that the array didn't overflow, if it did -> return error
	if(strlen(tmp) < MAX_PATH)
	{
		//make sure command wasn't empty
		if(cmd == NULL)
		{
			char message[] = "FILE NOT FOUND, PLEASE MAKE SURE IT EXISTS";
			errorMsg(message);
		}
		else
		{
			file = fopen((cwd, "/", cmd), "r");
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

void createFile(char* cwd, char* cmd)
{
    FILE *file;
    file = fopen((cwd,"/", cmd), "w");
    fclose(file);

    setFontGreen;
    printf("CREATED FILE: %s/%s\n", cwd, cmd);
    setFontWhite;
}


//called to change directory
void changeDir(char *dir)
{
    chdir(dir);
}
