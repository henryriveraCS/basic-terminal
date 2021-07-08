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
//value of cwd is updated by getCurrentDir
char* cwd;
//tmp is used to build a path and check whether an overflow state has been reached
char tmp[UNIX_MAX_PATH];

void clearIO(void)
{
    system("clear");
}

//this is used to build a path
char *buildPath(const char* dir, const char* fileName)
{
	//create array with length of tmpPath
	char *tmpPTR = tmp;
	strncat(tmpPTR, dir, (UNIX_MAX_PATH - strlen(cwd) -1));
	strncat(tmpPTR, "/", (UNIX_MAX_PATH - strlen("/") -1));
	strncat(tmpPTR, fileName, (UNIX_MAX_PATH - strlen(fileName) - 1));
	//*tmp = tmpPTR;
	return tmpPTR;
}

//used to get the directory as long as you provide the full path
char *getCurrentDir(void)
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
void readDir(void)
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

void readFile(const char* dir, const char* fileName)
{
    FILE *file;
    char c;
	char *tmp2 = buildPath(dir, fileName);
	//260 char length is the MAX_PATH of windows32 API library

	//verify that the array didn't overflow, if it did -> return error
	if(strnlen(tmp, UNIX_MAX_PATH) < UNIX_MAX_PATH)
	{
		//make sure command wasn't empty
		if(dir == NULL)
		{
			char message[] = "FILE NOT FOUND, PLEASE MAKE SURE IT EXISTS";
			errorMsg(message);
		}
		else
		{
			file = fopen(tmp2, "r");
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



void createFile(const char *dir, const char *fileName)
{
	char *tmp2 = buildPath(dir, fileName);
    FILE *file;
	//verify the path won't overflow
	if(strnlen(tmp2, UNIX_MAX_PATH) < UNIX_MAX_PATH)
	{
		file = fopen(tmp2, "w");
		fclose(file);

		setFontGreen;
		printf("CREATED FILE: %s\n", tmp2);
		setFontWhite;
	}
	else
	{
		char *msg = "OVERFLOW ON FILE CREATION, PLEASE MAKE SURE IT'S UNDER MAX_PATH LENGTH";
		errorMsg(msg);
	}
}

/*
void deleteFile(const char* dir, const char* fileName)
{

}

void makeDir(const char* dir, const char* dirName)
{

}

void copyDir(const char* cwd, const char* cmd)
{

}
*/

void copyFile(const char *dir, const char *oldFileName, const char *newFileName)
{
	/*
	FILE *oldTmp;
	FILE *newTmp;
	*/
	
	char *oldTmp = buildPath(dir, oldFileName);
	char *newTmp = buildPath(dir, newFileName);

	printf("\nOLD: %s\nNEW: %s\n", oldTmp, newTmp);
}


//called to change directory
void changeDir(char *dir)
{
    chdir(dir);
}
