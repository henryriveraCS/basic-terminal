#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>

//MACROS
//max path length allowed by win32 api
#define WIN_MAX_PATH 260
//max unix path just for consistency
#define UNIX_MAX_PATH 1024

//delcare all functions
void errorMsg(char *error);
void clearIO(void);
void printHelp();
char *getCurrentDir(void);
void changeDir(char *dir);
char *buildPath(char *tmp, const char *dir, const char*fileName);
void createDir(const char *dir, const char *dirName);
void createFile(const char *dir, const char *fileName);
void copyFile(const char *dir, const char *oldFileName, const char *newFileName);
void readFile(const char *dir, const char *fileName);
void readDir(void);
void copyDir(const char *cwd, const char *cmd);

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

void clearIO(void)
{
    system("clear");
}

//used to get the directory as long as you provide the full path
char *getCurrentDir(void)
{
    //use buffer to get the path
    char *BUF = getcwd(NULL, 0);
    if(BUF == NULL)
    {
        setFontRed;
        printf("\nERROR GETTING DIRECTORY: %s \n\n", BUF);
        setFontWhite;
    }
	cwd = BUF;
    return BUF;
}

//this is used to build a path
char *buildPath(char *tmp, const char *dir, const char *fileName)
{
	//copy to empty memory alloc first then append to that
	strncpy(tmp, dir, (UNIX_MAX_PATH - strlen(cwd) -1));
	strncat(tmp, "/", (UNIX_MAX_PATH - strlen("/") -1));
	strncat(tmp, fileName, (UNIX_MAX_PATH - strlen(fileName) - 1));

	return tmp;
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

void readFile(const char *dir, const char *fileName)
{
    FILE *file;
    char c;
	char tmp[UNIX_MAX_PATH];
	char *tmp2 = buildPath(tmp, dir, fileName);

	//verify that the array didn't overflow, if it did -> return error
	if(strnlen(tmp2, UNIX_MAX_PATH) < UNIX_MAX_PATH)
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
	char tmp[UNIX_MAX_PATH];
	char *tmp2 = buildPath(tmp, dir, fileName);

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

void removeFile(const char *dir, const char *fileName)
{
	char *tmp;
	tmp = (char *) malloc(UNIX_MAX_PATH);

	buildPath(tmp, dir, fileName);
	
	if(remove(tmp) == 0)
	{
		printf("DELETED: %s/%s\n", dir, fileName);
	}
	else
	{
		char *msg = "ERROR DELETING THE FILE. PLEASE MAKE SURE IT EXISTS";
		errorMsg(msg);
	}
	free(tmp);
}


void createDir(const char *dir, const char *dirName)
{
	DIR *newDir;
	char *dirPath;
	dirPath = (char *) malloc(UNIX_MAX_PATH);

	buildPath(dirPath, dir, dirName);
	newDir = opendir(dirPath);

	//check if dir exists 
	if( newDir == NULL )
	{
		mkdir(dirName, S_IRWXU);		
	}
	else
	{
		char *msg = "ERROR CREATING DIRECTORY. MAKE SURE IT DOESN'T ALREADY EXISTS";
		errorMsg(msg);
	}

}

void copyDir(const char *cwd, const char *cmd)
{

}

void copyFile(const char *dir, const char *oldFileName, const char *newFileName)
{
	FILE *oldFile;
	FILE *newFile;
	char *oldTmp;
	char *newTmp;

	oldTmp = (char *) malloc(UNIX_MAX_PATH);
	newTmp = (char *) malloc(UNIX_MAX_PATH);

	//update oldTmp + newTmp
	buildPath(oldTmp, dir, oldFileName);
	buildPath(newTmp, dir, newFileName);

	oldFile = fopen(oldTmp, "r");
	newFile = fopen(newTmp, "a");
	if(oldFile != NULL)
	{
		//while not at end of old file -> append the char to the new file
		char c;	
		while( (c = getc(oldFile)) != EOF )
		{
			fprintf(newFile, "%c", c);	
		}
	}
	else
	{
		char *msg = "EITHER ORIGINAL FILE PATH OR NEW FILE PATH IS OVER UNIX_MAX_PATH";
		errorMsg(msg);
	}

	printf("\nOLD FILE: %s\nNEW FILE: %s\n", oldTmp, newTmp);
	//free space after doing what you need to do
	fclose(newFile);
	fclose(oldFile);
	free(oldTmp);
	free(newTmp);
}


//called to change directory
void changeDir(char *dir)
{
    chdir(dir);
}
