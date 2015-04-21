#include "test.h"
#include <pthread.h> //for threading , link with lpthread

#define MAXBUF 1024
void *connection_handler(void *);

int main(int argc , char *argv[]){
  int PORT = atoi(argv[1]);
  int socket_desc , new_socket , c , *new_sock;
  struct sockaddr_in server , client;
  char *message;

  //Create socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);
  if (socket_desc == -1) printf("Could not create socket");

  //Prepare the sockaddr_in structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( PORT );

  //Bind
  if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0){
      puts("bind failed");
      return 1;
  }
  puts("bind done");

  //Listen
  listen(socket_desc , 3);

  //Accept and incoming connection
  puts("Waiting for incoming connections...");
  c = sizeof(struct sockaddr_in);
  while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) ){
      puts("Connection accepted");

      pthread_t sniffer_thread;
      new_sock = malloc(1);
      *new_sock = new_socket;

      if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0){
          perror("could not create thread");
          return 1;
      }

      //Now join the thread , so that we dont terminate before the thread
      //pthread_join( sniffer_thread , NULL);
      puts("Handler assigned");
  }

  if (new_socket<0){
      perror("accept failed");
      return 1;
  }

  return 0;
}

/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc){
  char buffer[MAXBUF];
  //Get the socket descriptor
  int sockfd = *(int*)socket_desc;
  int clientfd;
	struct sockaddr_in client_addr;
	int addrlen=sizeof(client_addr);
	char* token;
	//char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

	/*---accept a connection (creating a data pipe)---*/
	clientfd = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);


	recv(clientfd, buffer, 1024, 0);
	token = strtok(buffer," ");
	token = strtok(buffer," ");
  printf("%s\n",token);
	char string[100];
	strcpy(string, "www.");
	strcat(string,token);
	//Check for file existance
	//char* fileN = string;
	//strcat(fileN,".html");
	if(doesFileExist(string) != 1) getHTML(string);

	//Open file for sending
	FILE* site = fopen(string,"r");
	//send(clientfd,header,sizeof(header),0);
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
  return 0;
}
