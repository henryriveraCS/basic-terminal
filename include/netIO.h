#ifndef NETIO_H
#define NETIO_H

void printURL(char *url);
void downloadURL(char *url, char *fileName);
void printIP(char *ip, int len);
void convertCharToIP(char *ip, int len, int ipArr[4]);
//supports ipv4 for now
void ping (char *ip, int ipLength);
char *sendMsg (char *msg);
int wget (char *url);

#endif /* NETIO_H */
