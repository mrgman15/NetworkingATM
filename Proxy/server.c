#include "test.h"

#define MY_PORT		9999
#define MAXBUF		102400

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
//	while (1){
		int clientfd;
		struct sockaddr_in client_addr;
		int addrlen=sizeof(client_addr);
		char* token;

		/*---accept a connection (creating a data pipe)---*/
		clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
		/*---Echo back anything sent---*/
		recv(clientfd, buffer, 1024, 0);
		token = strtok(buffer," ");
		token = strtok(NULL," ");
		token = token + 1;
		char string[100];
		strcpy(string, "www.");
		strcat(string,token);
		printf("%s\n",string);
		//Check for file existance
		char* fileN = string;
		strcat(fileN,".html");
		if(doesFileExist(fileN) != 1) getHTML(string);

		//Open file for sending
		FILE* site = fopen(string,"r");
		bzero(buffer,sizeof(buffer));
		while(fgets(buffer,102400,site)!=NULL){
			send(clientfd, buffer,102400 , 0);
			bzero(buffer,sizeof(buffer));
			//printf("%s",buffer);
		}

		/*---Close data connection---*/
		close(clientfd);
//	}

	/*---Clean up (should never get here!)---*/
	close(sockfd);
	return 0;
}
