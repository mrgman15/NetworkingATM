#include "test.h"

#define MY_PORT		9999
#define MAXBUF		1024

int main(int Count, char *Strings[]){
	int sockfd;
	struct sockaddr_in self;
	char buffer[MAXBUF];

	/*---Create streaming socket---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
		perror("Socket");
		exit(errno);
	}

	/*---Initialize address/port structure---*/
	bzero(&self, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(MY_PORT);
	self.sin_addr.s_addr = INADDR_ANY;

	/*---Assign a port number to the socket---*/
    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 ){
		perror("socket--bind");
		exit(errno);
	}

	/*---Make it a "listening socket"---*/
	if ( listen(sockfd, 20) != 0 ){
		perror("socket--listen");
		exit(errno);
	}

	/*---Forever... ---*/
	while (1){
		int clientfd;
		struct sockaddr_in client_addr;
		int addrlen=sizeof(client_addr);
		char* token;
		//char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);


		recv(clientfd, buffer, 1024, 0);
		token = strtok(buffer," ");
		token = strtok(NULL," ");
		token = token + 1;
		char string[100];
		strcpy(string, "www.");
		strcat(string,token);
		//Check for file existance
		//char* fileN = string;
		//strcat(fileN,".html");
		if(doesFileExist(string) != 1) getHTML(string);

		//Open file for sending
		FILE* site = fopen(string,"r");
		send(clientfd,header,sizeof(header),0);
		bzero(buffer,sizeof(buffer));
		token = strtok(buffer, "<!DOCTYPE HTML>");
		send(clientfd,token,sizeof(token),0);
		printf("Header: %s\n",token);
		//send(clientfd,"<!DOCTYPE HTML>",14,0);
		token = strtok(buffer, "<!DOCTYPE HTML>");
		send(clientfd,token,sizeof(token),0);
		send(clientfd,buffer,sizeof(buffer),0);
		while(fgets(buffer,1024,site)!=NULL){
			send(clientfd, buffer, 1024, 0);
			//printf("%s",buffer);
			bzero(buffer,sizeof(buffer));
		}

		/*---Close data connection---*/
		close(clientfd);
	}

	/*---Clean up (should never get here!)---*/
	close(sockfd);
	return 0;
}