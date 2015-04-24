#include "proxy3.h"
#include <pthread.h> //for threading , link with lpthread

#define MAXBUF 1024
void *connection_handler(void *);

int main(int argc , char *argv[]){
  int PORT = atoi(argv[1]);
  int socket_desc, new_socket, c;
  struct sockaddr_in server, client;
  pthread_t sniffer_thread;
  int *new_sock;
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
  while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, &c)) ){
      puts("Connection accepted");

      new_sock = malloc(1);
      *new_sock = new_socket;

      if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*)new_sock) < 0){
          perror("could not create thread");
          return 1;
      }

      //Now join the thread , so that we dont terminate before the thread
      //pthread_join( sniffer_thread , NULL);
  }

  if (new_socket<0){
      perror("accept failed");
      return 1;
  }

  return 0;
}

//This will handle connection for each client
void *connection_handler(void *desc) {
  struct sockaddr_in client_addr;
  int addrlen = sizeof(client_addr);
  int socket_desc = *(int*)desc;
  char buffer[MAXBUF];
  char url[2048], filename[2048];
  char *temp = NULL;
  FILE* site;

  int n = recv(socket_desc, buffer, 1024, 0);

  //printf("\nClient Request:\n%s",buffer);

  temp = strtok(buffer," ");
  temp = strtok(NULL," ");
  strcpy(url, temp);
  memmove(&url[0], &url[1], strlen(url));

  //DEBUG: Should have "[url]".
  printf("Parsed URL: '%s'\n", url);

//FILENAME NEEDS TO HAVE NO '/'
  //temp = strtok(url, "/ ");
  //strcpy(filename, temp);

  //DEBUG: Should have "[url]" up to the first '/'.
//  printf("URL filename: '%s'\n", filename);

  //if cached don't download new version
  if (!doesFileExist(url))
    getHTML(url);
  else
    printf("Reading from cache...\n");

  //Open file for sending
  if (doesFileExist(url)) {
    site = fopen(url,"r");
    bzero(buffer,sizeof(buffer));
    //fgets(buffer,1024,site);
    if (doesFileExist(url))
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
      //printf("%s",buffer);
      bzero(buffer,sizeof(buffer));
    }
  }
  else {
    printf("Error opening file '%s'...\n", url);
    send(socket_desc,"HTTP/1.0 404 sendErrorErrorError\r\nContent-Type:text/html\r\n",sizeof("HTTP/1.0 404 sendErrorErrorError\r\nContent-Type:text/html\r\n"),0);
    //close(socket_desc);
    //return;
  }

  /*---Close data connection---*/
  close(socket_desc);
}
