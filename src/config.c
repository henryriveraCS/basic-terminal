#include <stdio.h>
#include <stdlib.h>

#include "./include/config.h"

#define UNIX_MAX_PATH 1024
#define WIN_MAX_PATH 260

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
                 "ls will display a list of files/directories in the current directory\n"
		 "'cd [DIRECTRY_NAME]' will move you into the specified DIRECTORY_NAME\n"
		 "'pwd' will show you the entire path to the current directory\n"
		 "'touch [FILE_NAME]' will create a file with the specified FILE_NAME\n"
		 "'clear' will clear all text from the terminal screen\n"
		 "'exit' will leave the BASIC TERMINAL application\n"
                 "'cp [FILE_NAME] [NEW_FILE_NAME]' will copy all contents from FILE_NAME into"
		 "the specified NEW_FILE_NAME\n"
		 "'mkdir [DIRECTORY_NAME]' will create a directory with the specified DIRECTORY_NAME\n"
    		 "'ping XXX.XXX.XXX.XX' will send a ping request to the specified IP address (only ipv4)\n"
    		 "'curl [URL]' will display html/data from the specified url\n"
    		 "'curl -d [URL] fileName' will download the data from the URL into the specified fileName"
    		 "at the current directory\n";
                 //"FOR MORE SPECIFIC USES USE 'HELP [COMMAND]'\n";
    printf("%s", cmds);
}
