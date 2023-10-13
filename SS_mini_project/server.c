/*
==============================================================================================================================
Name : server.c
Author : Aamika Mishra
Description : Writing server code managing all task of courses
==============================================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define MAX_CONNECTIONS 5

void *handle_client(void *client_socket);

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    pthread_t thread_id;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Server Binding done...\n");

    // Listen for incoming connections
    if (listen(server_socket, MAX_CONNECTIONS) == -1) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a client connection
        client_fd = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Accept failed");
            continue;
        }
        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket) != 0) {
            perror("Thread creation failed");
            close(client_socket);
        }
    }

    close(server_socket);
    return 0;
}

void *handle_client(void *arg) {
    int client_socket = *((int *)arg);
    char readBuffer[1000] , writeBuffer[1000];
    ssize_t readBytes , writeBytes;
    int choice;
    // send first data to client
    if(write(client_fd , INIT_MENU , strlen(INIT_MENU) == -1){
        perror("Write system call error");
    }
        bzero(readBuffer , sizeof(readBuffer));
        readBytes = read(client_fd , readBuffer , strlen(writeBuffer));
        if(readBuffer == -1)
        {
            perror("read system call error : while reading user input from client");
        }
        else if(readBuffer == 0)
                   printf("No data received from client");
        else
            choice = atoi(readBuffer);
        switch(choice)
        {
            case 1:
                admin;
                break;
            case 2:
                student;
                break;
            case 3:
                faculty;
                break;
            default:
                printf("please enter valid input");
                exit(1);
        }
                   

    close(client_fd);
    pthread_exit(NULL);
}

