#ifndef CONFIG_H
#define CONFIG_H

extern int UNIX_MAX_PATH;
extern int WIN_MAX_PATH;

void setFontWhite(void);
void setFontMagenta(void);
void setFontBlue(void);
void setFontGreen(void);
void setFontRed(void);

int getTTY(char *log);
void errorMsg(char *error);
void clearIO(void);
void printHelp(void);

#endif /* CONFIG_H */
