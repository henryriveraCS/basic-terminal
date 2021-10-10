#include "./include/fileIO.h"
#include "./include/config.h"
#include "./include/directoryIO.h"

#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

//used to read out whether a file is a direcotry or entry
struct stat filestat;
//value of cwd is updated by getCurrentDir
char* cwd; 

//used to get the directory as long as you provide the full path
char *getCurrentDir(void)
{
    //use buffer to get the path
    char *BUF = getcwd(NULL, 0);
    if(BUF == NULL)
    {
        setFontRed();
        printf("ERROR GETTING DIRECTORY: %s \n\n", BUF);
        setFontWhite();
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
            setFontGreen();
            printf("DIRECTORY: ");
            setFontWhite();
            printf("%s\n", entry->d_name);
        }
        else
        {
            setFontBlue();
            printf("FILE: ");
            setFontWhite();
            printf("%s\n", entry->d_name);
        }
    }
    closedir(folder);
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
            setFontGreen();
            printf("SUCESSFULLY DELETED DIRECTORY %s\n", dirName);
            setFontWhite();
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

//called to change directory
void changeDir(char *dir)
{
    chdir(dir);
}

