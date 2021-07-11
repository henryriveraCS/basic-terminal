#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <ctype.h>

//MACROS
//max path length allowed by win32 api
#define WIN_MAX_PATH 260
//max unix path just for consistency
#define UNIX_MAX_PATH 1024

//delcare all functions
void errorMsg(char *error);
void clearIO(void);
void printHelp();
bool isDir(const char *dirPath);
char *getCurrentDir(void);
void changeDir(char *dir);
char *buildPath(char *tmp, const char *dir, const char*fileName);
void createDir(const char *dir, const char *dirName);
void createFile(const char *dir, const char *fileName);
void copyFile(const char *dir, const char *oldFileName, const char *newFileName);
void readFile(const char *dir, const char *fileName);
void readDir(void);
void copyDir(const char *dir, const char *oldDirName, const char *newDirName, bool nestedCall);

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
        printf("ERROR GETTING DIRECTORY: %s \n\n", BUF);
        setFontWhite;
    }
	cwd = BUF;
    return BUF;
}

//this is used to build a path
char *buildPath(char *tmp, const char *dir, const char *fileName)
{
	//copy to empty memory alloc first then append to that
	if( dir != NULL)
	{
		strncpy(tmp, dir, (UNIX_MAX_PATH - strlen(cwd) -1));
		strncat(tmp, "/", (UNIX_MAX_PATH - strlen("/") -1));
		strncat(tmp, fileName, (UNIX_MAX_PATH - strlen(fileName) - 1));
	}
	else
	{
		strncpy(tmp, fileName, (UNIX_MAX_PATH - strlen(fileName) - 1));
	}

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
                 "ls\ncd\ntouch\npwd\nclear\nvim\nend\n";
    printf("%s", cmds);
}

bool isDir(const char *dirPath)
{
	DIR *dir;
	dir = opendir(dirPath);	
	if(dir == NULL)
	{
		return false;
	}
	else
	{
		return true;
	}
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
            setFontGreen;
            printf("DIRECTORY: ");
            setFontWhite;
            printf("%s\n", entry->d_name);
        }
        else
        {
            setFontBlue;
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
				setFontWhite;
			   	setFontWhite;
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

void removeDir(const char *dir, const char *dirName)
{
	char *tmp;
	tmp = (char *) malloc(UNIX_MAX_PATH);
	buildPath(tmp, dir, dirName);

	if( isDir(tmp) )
	{
		//try deleting the directory
		if( remove(tmp) == 0)
		{
			setFontGreen;
			printf("SUCESSFULLY DELETED DIRECTORY %s\n", dirName);
			setFontWhite;
		}
		else
		{
			char *msg = "ERROR DELETING DIRECTORY... PLEASE MAKE SURE IT EXISTS OR IT DOES NOT HAVE FILES INSIDE";
			errorMsg(msg);
		}
	}
	else
	{
		char *msg = "ERROR, THIS IS NOT A DIRECTORY. PLEASE USE *rm [FILE_NAME]* to delete a file";
		errorMsg(msg);
	}
	free(tmp);
}

void removeFile(const char *dir, const char *fileName)
{
	char *tmp;
	tmp = (char *) malloc(UNIX_MAX_PATH);

	buildPath(tmp, dir, fileName);
	//check if this is a file OR a directory(user must use rm -d for deleting directories)
	if( isDir(tmp) )
	{
		char *msg = "ERROR, THIS IS A DIRECTORY. PLEASE USE *rm -d [DIRECTORY]* to delete a directory";
		errorMsg(msg);
	}
	else
	{
		//try deleting the file -> if it fails then let the user know
		if ( remove(tmp) == 0 )
		{
			setFontGreen;
			printf("DELETED: %s/%s\n", dir, fileName);
			setFontWhite;
		}
		else
		{
			char *msg = "ERROR DELETING THE FILE. PLEASE MAKE SURE IT EXISTS";
			errorMsg(msg);
		}
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

void copyDir(const char *dir, const char *oldDirName, const char *newDirName, bool nestedCall)
{
    struct dirent *entry;
	//DIR *newDir;
	DIR *oldDir;
	FILE *oldFile;
	FILE *newFile;
	//char tmp[UNIX_MAX_PATH];
	char *oldTmp;
	char *newTmp;
	int oldCount = 0;
	int newCount = 0;
	char c;
	char *oldTmpFileName;
	char *newTmpFileName;
	char *oldTmpDirName;
	char *newTmpDirName;

	oldTmp = (char *) malloc(UNIX_MAX_PATH);
	newTmp = (char *) malloc(UNIX_MAX_PATH);
	
	oldTmpFileName = (char *) malloc(UNIX_MAX_PATH);
	oldTmpDirName = (char *) malloc(UNIX_MAX_PATH);

	newTmpFileName = (char *) malloc(UNIX_MAX_PATH);
	newTmpDirName =(char *) malloc(UNIX_MAX_PATH);

	if(nestedCall == false)
	{
		buildPath(oldTmp, dir, oldDirName);
		buildPath(newTmp, dir, newDirName);

		oldDir = opendir(oldTmp);
	}
	else
	{
		//because this is a nested loop, the following changes:
		//dir is  the original directory(where call was called from)
		//oldDirName is now the FULL directory that needs to be copied
		//newDirName is now the FULL directroy pointing to where the info needs to be pasted
		oldDir = opendir(oldDirName);
		strncpy(oldTmp, oldDirName, UNIX_MAX_PATH);
		strncpy(newTmp, newDirName, UNIX_MAX_PATH);
	}
	//check if old dir exists to copy from
	if( isDir(oldTmp) && oldDir != NULL )
	{
		//check if new dir exists 
		if( isDir(newTmp) )
		{
			//open the old directory -> iterate through each file/dir -> copy to new dir (if it doens't exists)
			while( (entry=readdir(oldDir)) )
			{
				//this is to prevent . and .. from being looked through during the copying process
				if( strncmp(entry->d_name, ".", UNIX_MAX_PATH) && strncmp(entry->d_name, "..", UNIX_MAX_PATH) )
				{
					oldCount ++; //track how many dirs/files in this directory
					strncpy(oldTmpFileName, oldTmp, UNIX_MAX_PATH);
					strncat(oldTmpFileName, "/", UNIX_MAX_PATH);
					strncat(oldTmpFileName, entry->d_name, UNIX_MAX_PATH);
					//build new path to use for appending files
					strncpy(newTmpFileName, newTmp, UNIX_MAX_PATH);
					strncat(newTmpFileName, "/", UNIX_MAX_PATH);
					strncat(newTmpFileName, entry->d_name, UNIX_MAX_PATH);
					//stat won't load unless you give it the full directory
					stat(oldTmpFileName, &filestat);
					//check if this is a dir
					if( S_ISDIR(filestat.st_mode) )
					{
						strncpy(oldTmpDirName, oldTmp, UNIX_MAX_PATH);
						strncat(oldTmpDirName, "/", UNIX_MAX_PATH);
						strncat(oldTmpDirName, entry->d_name, UNIX_MAX_PATH);
						printf("OLDTMP DIR NAME: %s\n", oldTmpDirName);

						strncpy(newTmpDirName, newTmp, UNIX_MAX_PATH);
						strncat(newTmpDirName, "/", UNIX_MAX_PATH);
						strncat(newTmpDirName, entry->d_name, UNIX_MAX_PATH);
						printf("NEWTMP DIR NAME: %s\n", newTmpDirName);

						//point copydir to the nested path in old dir and put it in new dir
						copyDir(oldTmp, oldTmpDirName, newTmpDirName, true);
						printf("%s\n", entry->d_name);
					}
					else
					{
						newCount ++;
						oldFile = fopen(oldTmpFileName, "r");
						//create new file in new dir -> copy content over
						createFile(newTmp, entry->d_name);
						newFile = fopen(newTmpFileName, "a");
						if( oldFile != NULL)
						{
							while( (c=getc(oldFile)) != EOF)
							{
								fprintf(newFile, "%c", c);	
							}
							printf("\n%i ITEMS WERE COPIED TO %s\n", newCount, newTmp);
							fclose(newFile);
						}
					}
				}
			}
		}
		else
		{
			if(nestedCall)
			{
				printf("CREATING DIR: %s\n", newDirName);
				createDir(NULL, newDirName);
			}
			else
			{
				printf("CREATING DIR: %s/%s\n", dir, newDirName);
				createDir(dir, newDirName);
			}
			while( (entry=readdir(oldDir)) )
			{
				if( strncmp(entry->d_name, ".", UNIX_MAX_PATH) && strncmp(entry->d_name, "..", UNIX_MAX_PATH) )
				{
					oldCount ++; //track how many dirs/files in this directory
					strncpy(oldTmpFileName, oldTmp, UNIX_MAX_PATH);
					strncat(oldTmpFileName, "/", UNIX_MAX_PATH);
					strncat(oldTmpFileName, entry->d_name, UNIX_MAX_PATH);

					//build new path to use for appending files
					strncpy(newTmpFileName, newTmp, UNIX_MAX_PATH);
					strncat(newTmpFileName, "/", UNIX_MAX_PATH);
					strncat(newTmpFileName, entry->d_name, UNIX_MAX_PATH);
					stat(oldTmpFileName, &filestat);

					if( S_ISDIR(filestat.st_mode) )
					{
						strncpy(oldTmpDirName, oldTmp, UNIX_MAX_PATH);
						strncat(oldTmpDirName, "/", UNIX_MAX_PATH);
						strncat(oldTmpDirName, entry->d_name, UNIX_MAX_PATH);

						strncpy(newTmpDirName, newTmp, UNIX_MAX_PATH);
						strncat(newTmpDirName, "/", UNIX_MAX_PATH);
						strncat(newTmpDirName, entry->d_name, UNIX_MAX_PATH);

						//point copydir to the nested path in old dir and put it in new dir
						copyDir(oldTmpDirName, oldTmpDirName, newTmpDirName, true);
					}
					else
					{
						newCount ++;
						oldFile = fopen(oldTmpFileName, "r");
						//create new file in new dir -> copy content over
						createFile(newTmp, entry->d_name);
						newFile = fopen(newTmpFileName, "a");
						if( oldFile != NULL)
						{
							while( (c=getc(oldFile)) != EOF)
							{
								fprintf(newFile, "%c", c);	
							}
							printf("\n%i ITEMS WERE COPIED TO %s\n", newCount, newTmp);
							fclose(newFile);
						}
					}
				}
			}
			//closedir(oldDir);
		}
	}
	else
	{
		char *msg = "DIRECTORY YOU ARE TRYING TO COPY DOES NOT EXISTS";
		errorMsg(msg);
	}
	free(newTmp);
	free(oldTmp);
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

	//double check that passed char* aren't actually directories
	if( isDir(oldTmp) == false && isDir(newTmp) == false)
	{
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
			fclose(newFile);
			fclose(oldFile);
		}
		printf("\nOLD FILE: %s\nNEW FILE: %s\n", oldTmp, newTmp);
	}
	else
	{
		char *msg = "EITHER ORIGINAL FILE PATH OR NEW FILE PATH IS OVER UNIX_MAX_PATH\n OR ONE OF THE ARGUMENTS IS A DIRECTORY";
		errorMsg(msg);
	}

	free(oldTmp);
	free(newTmp);
}


//called to change directory
void changeDir(char *dir)
{
    chdir(dir);
}
