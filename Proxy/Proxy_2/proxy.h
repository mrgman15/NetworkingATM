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
  int sockfd;

  char string[1024] = "GET / HTTP/1.1\r\nHost: ";
  strcat(string, website);
  strcat(string,":80\r\nConnection: keep-alive\r\n\r\n");
  char buffer[12256];
  char buffer2[12256];
  FILE* words = fopen("SwearFilter2.txt","r");
  char filter[248][23];
  int i = 0;
  while(i<248){
    fgets(filter[i++],22,words);
    printf("%s\n",filter[i-1]);
  }
  i=0;
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  printf("In getHTML: website = %s\n", website);

  getaddrinfo(website, "80", &hints, &res);
  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  connect(sockfd, res->ai_addr, res->ai_addrlen);
  int len = strlen(string);
  send(sockfd, string, len,0);
  //strcat(website,".html");
  int k= 0;
  while(recv(sockfd, buffer, 12256,0) !=0 || k<1){
    char* token = strtok(buffer," ");
    while(token!=NULL){
      while(i<248){
        if(strcmp(token, filter[i++]) == 0){
          strcat(buffer2,"BADWORD ");
        }
        else{
          strcat(buffer2,token);
        }
      }
      token = strtok(NULL," ");
    }
    putToFile(website, buffer2);
    bzero(buffer, sizeof(buffer));
    bzero(buffer2,sizeof(buffer2));
    //printf("%s",buffer);
    k++;
	}
	close(sockfd);
}
