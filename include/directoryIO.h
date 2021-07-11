#ifndef DIRECTORYIO_H
#define DIRECTORYIO_H

#include <stdbool.h>

bool isDir(const char *dirPath);
void readDir(void);
void copyDir(const char *dir, const char *oldDirName, const char *newDirName, bool nestedCall);
char *buildPath(char *tmp, const char *dir, const char*fileName);
char *getCurrentDir(void);
void removeDir(const char *dir, const char *dirName);
void changeDir(char *dir);
void createDir(const char *dir, const char *dirName);

#endif /* DIRECTORYIO_H */
