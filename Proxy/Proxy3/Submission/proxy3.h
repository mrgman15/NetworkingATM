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
#include <fcntl.h> // for open
#include <unistd.h> // for close


int doesFileExist(char *filename){
  struct stat   buffer;
  return (stat (filename, &buffer) == 0);
}


void putToFile(char* fileName, char* info){
	FILE* cache = fopen(fileName, "a");
	fprintf(cache,"%s",info);
	fclose(cache);
}


char* profanityFilter(char* buffer){
  FILE* words;
  char* filter = malloc(sizeof(char*));
  char *pos;
  size_t len = 0;
  int i;
  words = fopen("Words","r");
  while (getline(&filter, &len, words) != -1) {
    filter[strlen(filter)-1]='\0';
    while(1) {
      pos = (char*)strcasestr(buffer, filter);
      if (pos == NULL)
        break;
      for(i = 0; i < strlen(filter); i++)
        pos[i] = '*';
      pos = NULL;
    }
  }
  free(filter);
  fclose(words);
  //printf("%s\n",buffer);
  return buffer;
}

int isBlacklisted(char* url){
  FILE* blacklist;
  char* filter = malloc(sizeof(char*));
  char *pos;
  size_t len = 0;
  int i;
  blacklist = fopen("Blacklist","r");
  while (getline(&filter, &len, blacklist) != -1) {
    filter[strlen(filter)-1]='\0';
    if(strcmp(url,filter)==0) return 1;
  }
  free(filter);
  fclose(blacklist);
  //printf("%s\n",buffer);
  return 0;
}


void getHTML(char* website){
  struct addrinfo hints, *res;
  int sockfd, s = 0;
  char string[1024];

  strcat(string,"GET / HTTP/1.1\r\nHost: ");

  //strcpy(string, "GET http://");
  strcat(string, website);
  //strcat(string,":80\r\nConnection: keep-alive\r\n\r\n");
  strcat(string,":80\r\n\r\n");


  char buffer[24512];
  char buffer2[24512];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;


  s = getaddrinfo(website, "80", &hints, &res);

  if (s != 0) {
    printf("Error! getaddrinfo: %s\n", gai_strerror(s));
    //close(sockfd);
    return;
  }

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  connect(sockfd, res->ai_addr, res->ai_addrlen);
  int len = strlen(string);
  send(sockfd, string, len,0);

  while(recv(sockfd, buffer, 24512,0) !=0){
    strcpy(buffer2,profanityFilter(buffer));
    printf("buffer: %s\n",buffer2);
    putToFile(website, buffer2);
    bzero(buffer, sizeof(buffer));
    bzero(buffer2, sizeof(buffer2));
	}
	//close(sockfd);
}
