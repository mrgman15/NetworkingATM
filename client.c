#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "Parse.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
    n = read(sockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("%s\n",buffer);
    int loggedIn = 0;

    while(1){
        if(loggedIn == 0){
            printf("You must create and account or login\nCreate account: 101 First Last Pin DL SSN Email\nLogin: 201 First Pin\n");
        }
        else printf("Please enter a command");
        fgets(buffer,256,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        n = read(sockfd,buffer,255);
        char** message = parse(buffer);
        printf("%s",message[0]);

        switch(atoi(message[0])){
            case 103 :
                //Account creation failed show which invalid entry
                printf("Account creation failed.\n");
                break;
            case 104 :
                //Account was created log in
                printf("Account was created and you are now logged in. \n");
                loggedIn = 1;
                break;
            case 105 :
                //Account already exists
                printf("Account already exists. \n");
                break;
            case 203 :
                //Failed attempt to authenticat
                printf("Authentication failed. \n");
                break;
            case 204 :
                //Exceeded number of authentication attempts
                printf("Exceeded number of authentication attempts. \n");
                printf("You will be disconnected. \n");
                close(sockfd);
                break;
            case 205 :
                //Authentication succeeded 
                printf("Authentication succeeded! \n");
                printf("You are now logged in! \n");
                loggedIn = 1;
                break;
            case 303 :
                // Deposit worked, New balance is returned
                printf("Deposit succesful! \n");
                printf("Your new balance is $%s. \n", message[1]);
                break;
            case 304 :
                //Deposit failed
                printf("Deposit failed! \n");
                printf("Your balance is still $%s. \n", message[1]);
                break;

            case 305 :
                //Atm is full so return deposit
                printf("Deposit failed!. \n ");
                printf("Your deposit has been returned \n");
                printf("Your balance is still $%s. \n", message[1]);
                break;      
                
            case 403 :
                //Withdraw is successful
                printf("Your withdraw was successful! \n");
                printf("Your new balance is $%s. \n", message[1]);
                break;

            case 404 :
                //Withdraw was unsuccessful because of insufficient funds
                printf("Your withdrawal was unsuccessful! \n");
                printf("Insufficient funds. \n");
                printf("Your balance is still $%s. \n", message[1]);
                break;

            case 405 :
                //Not enough money in ATM for withdrawal
                printf("Your withdrawal was unsuccessful! \n");
                printf("Not enough money in ATM for that withdrawal amount. \n");
            break;

            case 503 :
                //Returns balance request
                printf("Your balance is $%s. \n", message[1]);
            break;

            case 603 :
                //Transactions ???? 
            break;

            case 703 :
                //Not enough money for stamps
                printf("Transaction failed. \n");
                printf("Insufficient funds to buy stamps. \n");
                break;

            case 704 :
            //Buying stamps was successful return new balance
                printf("Your transaction was successful! \n");
                printf("Your new balance is $%s. \n", message[1]);
                break;

            case 705 :
                //Not enough stamps left
                printf("Not enough stamps available for this transaction. \n");
                break;

            case 803 :
                //Successful logout
                loggedIn = 0;
                printf("Logout successful \n");
                printf("You are now logged out. \n");
                break;

            case 908 :
                //Missing entries
                printf("Your request was missing entries. \n");
                printf("Please try again. \n"); 
            break;

            case 909 :
                //Unknown error code received
                printf("Unknown error occurred. \n");
                printf("Please try again. \n");
                break;

            default :
                 printf("Error: Invalid Request. \n");
        }


        /*
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
        bzero(buffer,256);
        n = read(sockfd,buffer,255);
        if (n < 0) 
             error("ERROR reading from socket");
        printf("%s\n",buffer);
        */
    }
    close(sockfd);
    return 0;
}