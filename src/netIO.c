#include "./include/netIO.h"
#include "./include/config.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

//struct for IPv4 --- ipV6 will be added soon
typedef struct ipv4Hdr
{
	unsigned int h_len; //length for header
	unsigned int version; //version of IP(ipV4)
	unsigned char tos; //type of service
	unsigned short total_len; //total length of packet
	unsigned short id; //unique identifier
	unsigned short frag_flags; //flags that are set
	unsigned ttl; //time-to-live
	unsigned proto; //packets protocol (TCP, UDP, etc)
	unsigned short cheksum; //checksum of IP

	unsigned int sourceIP; //IP packet is being sent from
	unsigned int destIP; //IP packet is being sent to
} ipv4Header;

#define ICMP_ECHO 8
#define ICMP_ECHOREPLY 0
#define ICMP_MIN 8

int convertIntToBinary(int *ipArr, int length)
{
	return 0;
}

int convertCharToInt(char *ip, int len)
{
	return 0;
}

void printIP(char *ip, int len)
{
	printf("IP: ");
	for(int i=0; i<len ; i++)
	{
		printf("%c", ip[i]);
	}
	printf("\n");
}

void convertCharToIP(char *ip, int len, int ipArr[4])
{
	printIP(ip, len);
	char *tokens = strtok(ip, ".");
	/*
	E.G given *char ip = "192.168.1.1":
		Explanation:
		Take the first delmited token *char "192" -> convert it to an int 192 and assign 
		it to firstIP. Now that firstIP isn't NULL, we can move onto the next iteration by 
		NULLING "192" and moving onto *char "168" and repeating the same steps for secondIP.
			
	*/
	if(tokens != NULL)
	{
		ipArr[0] = atoi(tokens);
		tokens = strtok(NULL, ".");
	}
	if( tokens != NULL)
	{
		ipArr[1] = atoi(tokens);
		tokens = strtok(NULL, ".");
	}
	if( tokens != NULL)
	{
		ipArr[2] = atoi(tokens);
		tokens = strtok(NULL, ".");
	}
	if(tokens != NULL)
	{
		ipArr[3] = atoi(tokens);
		tokens = strtok(NULL, ".");
	}
	//in case anything is left, null it out
	if(tokens != NULL)
	{
		printf("TOO MANY '.' have been passed in, removing everything starting from: %s\n", tokens);
		tokens = NULL;
	}
}
	//null out all IP values so data doesn't carry over to another potential address
	//E.G if user pings 192.168.1.231 first then 10.0.1 (even tho it's a type) the program will return
	//10.0.1.231 (carried from last ping)

void ping(char *ip, int ipLength)
{
	//before doing anything related to ICMP - first convert the string into an
	//IP address int. (we do this by taking a basic IP char address like -
	//"10.0.2.1" and converting it to an int type into an array => [10, 0, 2, 1]
	//int ipSize = 4;
	int ipArr[] = {1234, 1234, 1234, 1234};
	convertCharToIP(ip, ipLength, ipArr);
	/*
	for (int i=0; i<ipSize; i++)
	{
		if(ipArr[i] != 1234)
		{
			printf("%d : %d\n", i, ipArr[i]);
		}
	}
	*/
	
	//int tries = 5;
	//int failed_tries = 0;
	//int bytes_sent, bytes_recv;
}
