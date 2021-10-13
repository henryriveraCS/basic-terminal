#include "./include/netIO.h"
#include "./include/config.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


//used to prep socket address struct for more uses(store data for later)
struct addrinfo
{
	int              ai_flags;     // AI_PASSIVE, AI_CANONNAME (address flags)
	int              ai_family;    // AF_INET, AF_INET6, AF_UNSPEC (defined socket family)
	int              ai_socktype;  // SOCK_STREAM, SOCK_DGRAM (TCP, UDP)
	int              ai_protocol;  // use 0 for "any"
	size_t           ai_addrlen;   // size of ai_addr in bytes
	struct sockaddr *ai_addr;      // struct sockaddr_in or _in6
	char            *ai_canonname; // full canonical hostname
	
	struct addrinfo *ai_next;      // linked list, next node
};

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

//used when default "curl http://etc.com" is called by terminal
void printURL(char *url)
{
	CURL *curl = curl_easy_init();
	if(curl)
	{
		//CURLcode res; // used for reading codes
		//start reading the URL
		curl_easy_setopt(curl, CURLOPT_URL, url);
		//curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		//res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
	}
	else
	{
		printf("Error loading the URL, please make sure the lcurl library is linked.\n");
	}
}

//used when "curl -d http://etc.com fileName" is called by terminal
void downloadURL(char *url, char *fileName)
{
	printf("Fetching: \n%s\n", url);

	//unsigned int downloads = 0;
	//int still_alive = 1;
	CURL *curl = curl_easy_init();
	if(curl)
	{
		CURLcode res;
		//start reading the URL
		curl_easy_setopt(curl, CURLOPT_URL, url);
		//curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP);
		//open file
		FILE *file = fopen( fileName, "w");
		//write to opened file
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		//close file in order to save read data
		fclose(file);
		printf("Successfully downloaded file\n");
	}
	else
	{
		printf("Error while downloading files, please make sure the lcurl library is linked.\n"
			"Please also validate that you are using a valid link.\n");
	}

}


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
	//int ipSize = 4;
	//int ipArr[] = {1234, 1234, 1234, 1234};
	struct sockaddr_in sa; //IPv4 socket address
	
	//store the ipv4 address from IP into sa
	int result = inet_pton(AF_INET, ip, &(sa.sin_addr));
	//returns 1 on success, 0 if src isn't valid, -1 on error
	switch(result)
	{
		case 0:
			printf("This address is not valid\n");
			break;
		case -1:
			printf("Error occured, please make sure the ip address is IPv4\n");
			break;
		default:
			printf("valid IP address: %s\n", ip);	
	}

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
