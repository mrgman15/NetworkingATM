#include "proxy.h"

#define MY_PORT		10005
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
	//while (1){
		int clientfd;
		struct sockaddr_in client_addr;
		int addrlen=sizeof(client_addr);
		char* token;

		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);

		recv(clientfd, buffer, 1024, 0);
		token = strtok(buffer," ");
		token = strtok(NULL," ");
		token = token + 1;
		char string[100];
		strcpy(string, "www.");
		strcat(string,token);
		//String = www.website.com

		//if cached don't download new version
		if(doesFileExist(string) != 1) getHTML(string);

		//Open file for sending
		FILE* site = fopen(string,"r");

		bzero(buffer,sizeof(buffer));
		fgets(buffer,1024,site);
		while(buffer[0] != '<'){
			send(clientfd,buffer,sizeof(buffer),0);
			printf("%s",buffer);
			bzero(buffer,sizeof(buffer));
			fgets(buffer,1024,site);
		}
		printf("%s",buffer);

		send(clientfd,buffer,sizeof(buffer),0);
		send(clientfd,"\r\n",sizeof("\r\n"),0);
		while(fgets(buffer,1024,site)!=NULL){
			send(clientfd, buffer, 1024, 0);
			printf("%s",buffer);
			bzero(buffer,sizeof(buffer));
		}
		/*---Close data connection---*/
		close(clientfd);
	//}

	/*---Clean up (should never get here!)---*/
	close(sockfd);
	return 0;
}
