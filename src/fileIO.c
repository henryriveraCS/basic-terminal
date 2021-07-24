#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

#include "./include/fileIO.h"
#include "./include/directoryIO.h"
#include "./include/config.h"

bool isFile(const char* filePath)
{
	FILE *tmp;
	tmp = fopen(filePath, "r");
	if(tmp == NULL)
	{
		return false;	
	}
	else
	{
		return true;
	}
	fclose(tmp);
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
			   	setFontWhite();
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

		setFontGreen();
		printf("CREATED FILE: %s\n", tmp2);
		setFontWhite();
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
			setFontGreen();
			printf("DELETED: %s/%s\n", dir, fileName);
			setFontWhite();
		}
		else
		{
			char *msg = "ERROR DELETING THE FILE. PLEASE MAKE SURE IT EXISTS";
			errorMsg(msg);
		}
	}
	free(tmp);
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
