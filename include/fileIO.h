#ifndef FILEIO_H
#define FILEIO_H

void removeFile(const char *dir, const char *fileName);
void createFile(const char *dir, const char *fileName);
void copyFile(const char *dir, const char *oldFileName, const char *newFileName);
void readFile(const char *dir, const char *fileName);

#endif /* FILEIO_H */
