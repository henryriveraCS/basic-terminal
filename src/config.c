#include <stdio.h>
#include <stdlib.h>

#include "./include/config.h"

int UNIX_MAX_PATH = 1024;
int WIN_MAX_PATH = 260;

void setFontWhite(void) { printf("\033[0m"); }
void setFontMagenta(void) { printf("\033[35m"); }
void setFontBlue(void) { printf("\033[0;34m"); }
void setFontGreen(void) { printf("\033[32m"); }
void setFontRed(void) { printf("\033[31m"); }

void clearIO(void)
{
    system("clear");
}

void errorMsg(char *error)
{
    setFontRed();
    printf("\nWARNING: ");
    setFontWhite();
    printf("%s\n", error);
}  

void printHelp(void)                                                                                                              
{
    char *cmds = "THE FOLLOWING COMMANDS ARE AVAILABLE ON THIS TERMINAL:\n"
                 "ls\ncd [DIRECTRY_NAME]\ntouch [FILE_NAME]\npwd\nclear\nvim [FILE_NAME]\nend\n"
                 "cp [ORIGINAL_FILE_NAME] [COPIED_FILE_NAME]\nmkdir [DIRECTORY_NAME]\n"
                 "FOR MORE SPECIFIC USES USE HELP [COMMAND]";
    printf("%s", cmds);
}
