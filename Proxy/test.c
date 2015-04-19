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


int main(){

  struct addrinfo hints, *res;
  int sockfd;

  // first, load up address structs with getaddrinfo():

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo("www.example.com", "80", &hints, &res);

  // make a socket:

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  // connect!
  char string[] = "GET / HTTP/1.1\r\nHost: www.example.com:80\r\nConnection: keep-alive\r\n\r\n";
  char buffer[2048];

  connect(sockfd, res->ai_addr, res->ai_addrlen);
  send(sockfd, string, strlen(string),0);
  int i=0;
  while(strlen(buffer)!=0 || i<1){
    bzero(buffer, sizeof(buffer));
    recv(sockfd, buffer, 2048,0);
    printf("%s\n",buffer);
    i++;
  }




  return 0;
}
