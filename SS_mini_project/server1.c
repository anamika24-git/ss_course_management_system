#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


#include "server_macro.h"
#include "admin_functionality.h"

void client_handler(int client_FD);
int main()
{
    int socket_FD, client_FD, listen_status, bind_status;
    struct sockaddr_in serveraddress,clientaddress;
    // Create socket
    socket_FD = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_FD == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    // Initialize server address structure
    memset(&serveraddress , 0 ,sizeof(serveraddress));
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_port = htons(8080);
    
    // server_FD has became the server now
    
    //bind address to specific IP & port
    bind_status = bind(socket_FD, (struct sockaddr *)&serveraddress, sizeof(serveraddress));
    if(bind_status == -1){
        perror("Bind failed\n");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    listen_status = listen(socket_FD, 5);
     if(listen_status == -1)
     {
        perror("Listen failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port 8080\n" );
    
    // Many clients reruest can be serve conurrently using fork and accept in loop
    socklen_t  clientSize;
           while(1)
           {
               clientSize = sizeof(clientaddress);
               client_FD = accept(socket_FD, (struct sockaddr *)&clientaddress, (socklen_t *)&clientSize);
               if (client_FD == -1)
               {
                   perror("Error while connecting to client!\n");
                   close(socket_FD);
               }
               else
               {
                   if (!fork())
                   {
                       // Child will enter this branch
                       close(socket_FD); // close as raw descriptor not use for handling client anymore
                       client_handler(client_FD);
                       exit(0);
                   }
               }
               
           }

        close(socket_FD);
    
       }

       void client_handler(int client_FD)
    {
           printf("Hello I am server\n");
           // send first data to client
           if(write(client_FD , INITIAL_SETUP , strlen(INITIAL_SETUP)) == -1) {
               perror("Write system call error\n");
           }
           char readBuffer[1000] , writeBuffer[1000];
           ssize_t readBytes , writeBytes;
           int choice;
         
               bzero(readBuffer , sizeof(readBuffer));
               readBytes = read(client_FD , readBuffer , sizeof(readBuffer));
               if(readBytes == -1)
               {
                   perror("read system call error : while reading user input from client\n");
               }
               else if(readBytes == 0)
                          printf("No data received from client\n");
               else
                   choice = atoi(readBuffer);
               switch(choice)
               {
                   case 1:
                       admin_menu(client_FD);
                       break;
                   case 2:
                       printf("student\n");
                       break;
                   case 3:
                       printf("faculty\n");;
                       break;
                   default:
                       printf("please enter valid input\n");
                       exit(0);
               }
                          
        }
           
    
    
