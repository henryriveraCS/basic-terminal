#ifndef FLORIDAVIM_H
#define FLORIDAVIM_H
#include <stdio.h>

int launchFloridaVim(char *dir, char *FileName);

struct editorDisplay
{
	int screenRows;
	int screenCols;
	int currentRow;
	int currentCol;
	char *fileName;

};

struct editorKey
{
	//available movements
	char up;
	char down;
	char left;
	char right;
	//this prints the current status of file ("editing:", "menu:", etc)
	char statusMsg[50];

	//tracks current mouse position
	int currentCol;
	int currentRow;
};

#endif /* FLORIDAVIM_H */
