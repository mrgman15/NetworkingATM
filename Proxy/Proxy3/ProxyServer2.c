#include "proxy3.h"
#include <pthread.h> //for threading , link with lpthread

void *connection_handler(void *);

int main(int argc , char *argv[]){
  int PORT = atoi(argv[1]);
  int socket_desc, new_socket;
  socklen_t c;
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
  while((new_socket = accept(socket_desc, (struct sockaddr *)&client, &c))){
      if(new_socket == NULL) continue;
      puts("Connection accepted");

      new_sock = malloc(1);
      *new_sock = new_socket;

      if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*)new_sock) < 0){
          perror("could not create thread");
          //return 1;
      }

      //Now join the thread , so that we dont terminate before the thread
      pthread_join( sniffer_thread , NULL);
  }

  if (new_socket<0){
      perror("accept failed");
      //return 1;
  }

  return 0;
}

//This will handle connection for each client
void *connection_handler(void *desc) {
  struct sockaddr_in client_addr;
  int addrlen = sizeof(client_addr);
  int socket_desc = *(int*)desc;
  char buffer[8096];
  char url[2048], filename[2048];
  char *temp = NULL;
  FILE* site;

  int n = recv(socket_desc, buffer, 1024, 0);
  //printf("BUffer: %s",buffer);
  //printf("\nClient Request:\n%s",buffer);

  temp = strtok(buffer," ");
  temp = strtok(NULL,"/ ");
  strcpy(url, temp);

  if(isBlacklisted(url)){
    printf("%s is BLACKLISTED\n",url);
    send(socket_desc,"HTTP/1.1 404 sendErrorErrorError\r\n\r\n",sizeof("HTTP/1.1 404 sendErrorErrorError\r\n\r\n"),0);
    printf("Closing connection\n");
    close(socket_desc);
    free(desc);
    return 0;
  }

  printf("URL: '%s'\n", url);

  //if cached don't download new version
  if (!doesFileExist(url)) getHTML(url);

  //Open file for sending
    printf("Reading from cache...\n");
    site = fopen(url,"r");
    bzero(buffer,sizeof(buffer));
    //fgets(buffer,1024,site);
    if (doesFileExist(url))
      while(fgets(buffer,8096,site) != NULL){
        if(buffer[0] == '<' || buffer[1] == '<') break;
        //send(socket_desc,buffer,sizeof(buffer),0);
        //printf("%s",buffer);
        bzero(buffer,sizeof(buffer));
      }
    //printf("%s",buffer);
    send(socket_desc,"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n",sizeof("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"),0);
    //send(socket_desc,"<!DOCTYPE HTML>",sizeof("<!DOCTYPE HTML>"),0);

    while(1){
      send(socket_desc, buffer,sizeof(buffer), 0);
      if(buffer[0] == '\0') break;
      bzero(buffer,sizeof(buffer));
      if(fgets(buffer,8096,site) == NULL) break;
      //printf("%s\n",buffer);
      //strcpy(buffer,profanityFilter(buffer));
    }
  /*---Close data connection---*/
  //close(socket_desc)
  printf("Closing connection\n");
  fclose(site);
  close(socket_desc);
  free(desc);
  return 0;
}
