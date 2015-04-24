#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <stdlib.h>


int doesFileExist(char *filename){
  struct stat   buffer;
  return (stat (filename, &buffer) == 0);
}


void putToFile(char* fileName, char* info){
	FILE* cache = fopen(fileName, "a");
	fprintf(cache,"%s",info);
	fclose(cache);
}

void getHTML(char* website){
  struct addrinfo hints, *res;
  int sockfd, s = 0;
  char string[1024];

  strcat(string,"GET / HTTP/1.1\r\nHost: ");

  //strcpy(string, "GET http://");
  strcat(string, website);
  strcat(string,":80\r\nConnection: keep-alive\r\n\r\n");
  //strcat(string,"\r\nConnection: keep-alive\r\n\r\n");


  char buffer[12256];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;


  s = getaddrinfo(website, "80", &hints, &res);

  if (s != 0) {
    printf("Error! getaddrinfo: %s\n", gai_strerror(s));
    close(sockfd);
    return;
  }

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  connect(sockfd, res->ai_addr, res->ai_addrlen);
  int len = strlen(string);
  send(sockfd, string, len,0);

  while(recv(sockfd, buffer, 12256,0) !=0){
    putToFile(website, buffer);
    bzero(buffer, sizeof(buffer));
	}
	close(sockfd);
}
