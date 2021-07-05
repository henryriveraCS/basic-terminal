//font colors
void setFontWhite() { printf("\033[0m"); }
void setFontMagenta(){ printf("\033[35m"); }
void setFontBlue() { printf("\033[0;34m"); }
void setFontGreen() { printf("\033[32m"); }
void setFontRed() { printf("\033[31m"); }

//used to read out whether a file is a direcotry or entry
struct stat filestat;

void errorMsg(char *error)
{
	setFontRed();
	printf("WARNING: ");
	setFontWhite();
	printf("%s", error);
}   

void printHelp()
{
    char *cmds = "THE FOLLOWING COMMANDS ARE AVAILABLE ON THIS TERMINAL:\n"
                 "ls\ncd\ntouch\npwd\nclear\nvim\nend OR exit";
    printf("%s", cmds);
}

//used to get the directory as long as you provide the full path
char *getCurrentDir()
{
    //use buffer to get the path
    char *BUF = getcwd(NULL, 0);
    if(BUF == NULL)
    {
        setFontRed();
        printf("ERROR GETTING DIRECTORY: %s \n", BUF);
        setFontWhite();
    }
    //currentDir = BUF;
    return BUF;
}

//reading files in the current dir
void readDir(char dir)
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
            setFontMagenta();
            printf("DIRECTORY: ");
            setFontWhite();
            printf("%s\n", entry->d_name);
        }
        else
        {
            setFontGreen();
            printf("FILE: ");
            setFontWhite();
            printf("%s\n", entry->d_name);
        }
    }
    closedir(folder);
}

void readFile(char* cmd)
{
    FILE *file;
    char *cwd = getCurrentDir();
    char c;
    //check if file exists, otherwise handle the error
    if(fopen((cwd, "/", cmd), "r") == NULL)
    {
        char message[] = "FILE NOT FOUND, PLEASE MAKE SURE IT EXISTS\n";
        errorMsg(message);
    }
    else
    {
        file = fopen((cwd, "/", cmd), "r");
        //while you can still read characters in the file(not end of file)
        while( (c = getc(file)) != EOF)
        {
            printf("%c", c);
        }
        printf("\n");
    }
}


void clearIO()
{
    system("clear");
}

void createFile(char* cmd)
{
    FILE *file;
    char *cwd  = getCurrentDir();
    file = fopen((cwd,"/", cmd), "w");
    fclose(file);

    setFontGreen();
    printf("CREATED FILE: %s/%s\n", cwd, cmd);
    setFontWhite();
}


//called to change directory
void changeDir(char *dir)
{
    chdir(dir);
}
