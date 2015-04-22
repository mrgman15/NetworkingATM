#include "proxy.h"
#include <pthread.h> //for threading , link with lpthread

#define MAXBUF 1024
void *connection_handler(void *);

int main(int argc , char *argv[]){
  int PORT = atoi(argv[1]);
  int socket_desc , new_socket , c , new_sock;
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
      printf("%d",socket_desc);
      new_sock = new_socket;
      printf("%d",new_sock);

      if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*)new_sock) < 0){
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
void *connection_handler(void *desc){
  char buffer[MAXBUF];
  int socket_desc = desc;

  printf("socket dec: %d\n", socket_desc);
  //struct sockaddr_in client_addr;
  //int addrlen=sizeof(client_addr);
  char* token;
  char* temp;
  /*---accept a connection (creating a data pipe)---*/
  //socket_desc = accept(socket_desc, (struct sockaddr*)&client_addr, &addrlen);

  int n = recv(socket_desc, buffer, 1024, 0);
  perror(n);
  printf("%s\n",buffer);

  token = strtok(buffer," ");
  token = strtok(NULL," ");
  temp = strtok(token, "/");
  temp = strtok(NULL,"\0");
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
  while(buffer[0] != '<' && buffer[1] != '!'){
    //send(socket_desc,buffer,sizeof(buffer),0);
    //printf("%s",buffer);
    bzero(buffer,sizeof(buffer));
    fgets(buffer,1024,site);
  }
  //printf("%s",buffer);
  send(socket_desc,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n",sizeof("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"),0);
  //send(socket_desc,"<!DOCTYPE HTML>",sizeof("<!DOCTYPE HTML>"),0);
  send(socket_desc,buffer,sizeof(buffer),0);
  while(fgets(buffer,1024,site)!=NULL){
    send(socket_desc, buffer, 1024, 0);
    printf("%s",buffer);
    bzero(buffer,sizeof(buffer));
  }
  /*---Close data connection---*/
  close(socket_desc);
}
