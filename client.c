/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3510" // the port client will be connecting to 

#define MAXDATASIZE 255 // max number of bytes we can get at once 

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
int sockfd, numbytes;  
char buf[MAXDATASIZE];
struct addrinfo hints, *servinfo, *p;
int rv;
char s[INET6_ADDRSTRLEN];
int new_fd;
char input1[100];
char input2[100];
char c;
int i;
FILE* fp;
char check[MAXDATASIZE];

	//if (argc != 2) {
	//   fprintf(stderr,"usage: client hostname\n");
	//    exit(1);
	//}

while(1){
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo("127.0.0.1", PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: failed to connect\n");
		return 2;
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof s);

	freeaddrinfo(servinfo); // all done with this structure

		printf("client367:");
		//get the first word
		scanf("%s",input1);
		if(strcmp(input1,"list")==0)
			send(sockfd, "list",4,0);
		else if(strcmp(input1,"help")==0){
			printf("Commands:\n");
			printf("\tlist: List all files in server directory\n");
			printf("\tcheck <filename>: Check for file on server\n");
			printf("\tdisplay <filename>: Print contents of a file\n");
			printf("\tdownload <filename>: Download file from server\n");
			printf("\tquit: Exit program\n");
		}
		else if(strcmp(input1,"quit")==0)
			exit(0);
		else{
			//get the second word
			scanf("%s",input2);

			if(strcmp(input1,"check")==0){
				send(sockfd, "check",5,0);
				send(sockfd,input2,strlen(input2),0);
			}
			if(strcmp(input1,"display")==0){
				send(sockfd,"get",3,0);
				send(sockfd,input2,strlen(input2),0);
			}
			if(strcmp(input1,"download")==0){
				send(sockfd,"get",3,0);
				send(sockfd,input2,strlen(input2),0);
				numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0);
				buf[numbytes] = '\0';
				//check for file not found
				strcpy(check,"File <");
				strcat(check,input2);
				strcat(check,"> is not found");
				if(strcmp(buf,check)==0)
					printf("%s\n",buf);
				//check if filename exists locally
				else if(fopen(input2,"r")){
					while(getchar()!='\n');
					printf("File with name %s exists!\nOverwrite?(y/n)",input2);
					c=getchar();
					if(c=='y'){
						printf("Downloading file...");
						fp=fopen(input2,"w");
						fprintf(fp,"%s",buf);
						while(numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)){
							buf[numbytes] = '\0';
							fprintf(fp,"%s",buf);
						}
						fclose(fp);
						printf("complete!\n");
					}
					else if(c!='n')
						printf("Invalid answer.\n");
				}
				else{
					printf("Downloading file...");
					fp=fopen(input2,"w");
					fprintf(fp,"%s",buf);
					while(numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)){
						buf[numbytes] = '\0';
						fprintf(fp,"%s",buf);
					}
					fclose(fp);
					printf("complete!\n");
				}
			}
		}
	
		if(strcmp(input1,"help")!=0&&strcmp(input1,"download")!=0)
			while(numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)){
				buf[numbytes] = '\0';
				printf("%s",buf);
			}
		printf("\n");

}
close(sockfd);
}

