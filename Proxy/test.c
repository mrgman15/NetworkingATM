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

void putToFile(char* fileName, char* info);
void getHTML(char* website);


int main(){
  getHTML("www.example.com");

  return 0;
}


void putToFile(char* fileName, char* info){
	FILE* cache = fopen(fileName, "a");
	fprintf(cache,"%s",info);
	fclose(cache);
}

void getHTML(char* website){
  struct addrinfo hints, *res;
  int sockfd;

  char string[1024] = "GET / HTTP/1.1\r\nHost: ";
  strcat(string, website);
  strcat(string,":80\r\nConnection: keep-alive\r\n\r\n");
  char buffer[2048];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo("www.example.com", "80", &hints, &res);
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  connect(sockfd, res->ai_addr, res->ai_addrlen);

  send(sockfd, string, strlen(string),0);
  int i=0;
  while(strlen(buffer)!=0 || i<1){
    bzero(buffer, sizeof(buffer));
    recv(sockfd, buffer, 2048,0);
    putToFile(website, buffer);
    //printf("%s",buffer);
    i++;
	}
}
