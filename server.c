#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>   
#include "Parse.h" //write
 
#include<pthread.h> //for threading , link with lpthread
#define PORT 10109
 
void *connection_handler(void *);
 
int main(int argc , char *argv[])
{
    int socket_desc , new_socket , c , *new_sock;
    struct sockaddr_in server , client;
    char *message;
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( PORT );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        puts("Connection accepted");
         
        //Reply to the client
        message = "Hello Client , I have received your connection. And now I will assign a handler for you\n";
        write(new_socket , message , strlen(message));
         
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = new_socket;
         
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( sniffer_thread , NULL);
        puts("Handler assigned");
    }
     
    if (new_socket<0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
     
    char *message;
    char buffer[256];
    char* repeat;
    int loginFailure = 0, withdraw, deposit, amount;
    char* loginInfo;
    char* rbuffer;
    char* holder;

    char** incoming = parse(buffer);
    

    while(strcmp(incoming[0],"quit")){
        switch(atoi(incoming[0])){
            case 101: //Create Account
                if(validateCreate(incoming) == 0){
                    //Account Already Exists
                    if(access(createFileName(incoming), F_OK ) == -1 ){
                        rbuffer = "105";
                        write(sock , rbuffer , strlen(rbuffer));
                    }
                    else{
                        //Account Creation Success
                        loginInfo = createFileName(incoming);
                        putToFile(loginInfo,incoming);
                        rbuffer = "104";
                        write(sock , rbuffer , strlen(rbuffer));
                    }
                }
                else{
                    //Account Creation Failed
                    rbuffer = "103";
                    write(sock , rbuffer , strlen(rbuffer));
                }
                break;
            case 201: //Authenticate Login
                //Authentication Failure
                if(validateLogin(incoming)==1){
                    rbuffer = "203";
                    write(sock , rbuffer , strlen(rbuffer));
                    loginFailure++;
                    break;
                }

                //Authentication Exceeded
                if(loginFailure > 10){
                    rbuffer = "204";
                    write(sock , rbuffer , strlen(rbuffer));
                    break;
                }

                //Authentication Success
                loginInfo = createFileName(incoming);
                rbuffer = "205";
                write(sock , rbuffer , strlen(rbuffer));
                break;

            case 301:
                //Deposit Failed
                if(validateAmount(incoming[1]) == 1){
                    incoming = parseFile(loginInfo);
                    rbuffer = "304 ";
                    strcat(buffer, incoming[6]);
                    write(sock , rbuffer , strlen(rbuffer));
                    break;
                }
                //Deposit Success
                deposit = atoi(incoming[1]);
                addTransaction(incoming,"Deposit:",incoming[1]);
                incoming = parseFile(loginInfo);
                deposit += atoi(incoming[6]);
                sprintf(incoming[6], "%d", deposit);
                putToFile(loginInfo,incoming);
                rbuffer = "303 ";
                strcat(rbuffer, incoming[6]);
                write(sock , rbuffer , strlen(rbuffer));
                break;

            case 302:
                //ATM Full
                incoming = parseFile(loginInfo);
                rbuffer = "305 ";
                strcat(rbuffer, incoming[6]);
                write(sock , rbuffer , strlen(rbuffer));
                break;

            case 401:
                //Invalid Entry
                if(validateAmount(incoming[1]) == 1){
                    rbuffer = "909";
                    write(sock , rbuffer , strlen(rbuffer));
                    break;
                }

                //Valid Entry
                withdraw = atoi(incoming[1]);
                holder = incoming[1];
                incoming = parseFile(loginInfo);

                //Withdraw Failure
                if(atoi(incoming[6]) < withdraw){
                    incoming = parseFile(loginInfo);
                    rbuffer = "404 ";
                    strcat(rbuffer, incoming[6]);
                    write(sock , rbuffer , strlen(rbuffer));
                    break;
                }
                //Withdraw Success
                addTransaction(incoming,"Withdraw:",holder);
                withdraw *= (-1);
                withdraw += atoi(incoming[6]);
                sprintf(incoming[6], "%d", withdraw);
                putToFile(loginInfo, incoming);
                rbuffer = "403 ";
                strcat(rbuffer, incoming[6]);
                write(sock , rbuffer , strlen(rbuffer));
                break;

            case 402:
                //ATM Empty
                rbuffer = "402 ";
                incoming = parseFile(loginInfo);
                strcat(rbuffer, incoming[6]);
                write(sock , rbuffer , strlen(rbuffer));
                break;

            case 501:
                rbuffer = "503 ";
                incoming = parseFile(loginInfo);
                strcat(rbuffer, incoming[6]);
                write(sock , rbuffer , strlen(rbuffer));
                break;

            case 601:
                //Return n Previous Transactions
                incoming = parseFile(loginInfo);
                int k=7;
                while(incoming[k] != '\0'){
                    k++;
                }
                rbuffer = "603 ";
                strcat(rbuffer, (k+48));
                strcat(rbuffer, " ");
                k=7;
                while(incoming[k] != '\0'){
                    strcat(rbuffer, incoming[k]);
                    strcat(rbuffer, " ");
                    k++;
                }

                break;

            case 701:
                amount = atoi(incoming[1]);
                holder = incoming[1];
        		incoming = parseFile(loginInfo);
        				
        		//Not enough funds to buy stamps
        		if(atoi(incoming[6]) < amount){
        			rbuffer = "703";
        			write(sock , rbuffer , strlen(rbuffer));
        			break;
        		}	 
        		//Stamps bought
                addTransaction(incoming,"Stamps",holder);
        		amount *= (-1);
        		amount += atoi(incoming[6]);
        		sprintf(incoming[6], "%d", amount);
        		putToFile(loginInfo, incoming);
        		rbuffer = "704 ";
        		strcat(rbuffer, incoming[6]);
                write(sock , rbuffer , strlen(rbuffer));
                break;
                
            case 702:
                rbuffer = "705";
                write(sock , rbuffer , strlen(rbuffer));
                break;

            case 801:
                strcpy(incoming[0],"quit");
                rbuffer = "803";
                write(sock , rbuffer , strlen(rbuffer));
                break;

            default:
                rbuffer = "909";
                write(sock , rbuffer , strlen(rbuffer));
                break;

        }
    }

    free(socket_desc); 
    return 0;
}
