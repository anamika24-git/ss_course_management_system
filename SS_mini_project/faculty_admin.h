#include "server_macro.h"
#include "./file_data/faculty.h"
#include "./file_data/student.h"
#include<fcntl.h>
#include<string.h>

struct Faculty facul;
char writeBuffer[1500] , readBuffer[50];
void add_faculty(int client_FD);
void view_faculty_details(int client_FD);
void modify_faculty(int client_FD);

//Add faculty
void add_faculty(int client_FD)
{
    int faculty_fd = open("faculty_data.txt", O_RDWR | O_CREAT, 0644);

    if (faculty_fd == -1) {
        perror("Error opening the file");
        // Handle the error, maybe exit the function or the program
        return;
    }
    //adding faculty id
    memset(writeBuffer, 0, sizeof(writeBuffer)); // to clear writeBuffer content
    memset(readBuffer, 0, sizeof(readBuffer));  //to clear readbuffer content
    if(write(client_FD,FACULTY_ID, strlen(FACULTY_ID)) == -1)
        perror("Issue on writing faculty id on client side");
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
        perror("Issue on reading faculty id from client side");
    else {
        printf("Received faculty id: %s\n", readBuffer);
        strcpy(facul.faculty_id,readBuffer);
    }
    
    //adding faculty name
    memset(writeBuffer, 0, sizeof(writeBuffer)); // to clear writeBuffer content
    memset(readBuffer, 0, sizeof(readBuffer));  //to clear readbuffer content
    if(write(client_FD,FACULTY_NAME, strlen(FACULTY_NAME)) == -1)
        perror("Issue on writing faculty name on client side");
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
        perror("Issue on reading faculty name  from client side");
    else{
        printf("Received faculty name: %s\n", readBuffer);
        strcpy(facul.faculty_name, readBuffer);
    }
    //adding faculty department
    memset(writeBuffer, 0, sizeof(writeBuffer)); // to clear writeBuffer content
    memset(readBuffer, 0, sizeof(readBuffer));  //to clear readbuffer content
    if(write(client_FD,FACULTY_DEPARTMENT, strlen(FACULTY_DEPARTMENT)) == -1)
        perror("Issue on writing faculty department on client side");
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
        perror("Issue on reading faculty department  from client side");
    else
    {
        printf("Received faculty department: %s\n", readBuffer);
        strcpy(facul.faculty_department, readBuffer);
    }
    //set faculty password
    char pwd[10];
    strcat(pwd , facul.faculty_id);
    strcat(pwd , facul.faculty_department);
    strcat(facul.faculty_pwd , pwd);
    printf("Faculty password is set: %s\n", readBuffer);
    // writing faculty info in db file
    lseek(faculty_fd, 0, SEEK_END );
    printf("faculty_db\n");
    write(faculty_fd, &facul, sizeof(struct Faculty));
    close(faculty_fd);
    }


void view_faculty_details(int client_FD)
{
    struct Faculty display_faculty;
    int faculty_fd = open("faculty_data.txt", O_RDONLY, 0644);
    if (faculty_fd == -1) {
        perror("Error opening the file");
        // Handle the error, maybe exit the function or the program
        return;
    }
    if(write(client_FD , DISPLAYF , strlen(DISPLAYF)) == -1) {
        perror("Write system call error display function faculty\n");
    }
    memset(readBuffer, 0, sizeof(readBuffer));
    memset(writeBuffer, 0, sizeof(writeBuffer));
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1) {
    perror("read system call error display function faculty: while reading user input from client\n");
    }
    lseek(faculty_fd , 0 , SEEK_SET);
    // read each student data and print
    if(strcmp(readBuffer,"every") == 0) {
        while(read(faculty_fd , &display_faculty , sizeof(struct Faculty)) !=0 )
        {
            strcat(writeBuffer , "-----------------------------------\n");
            strcat(writeBuffer , " Faculty id: ");
            strcat(writeBuffer , display_faculty.faculty_id);
            strcat(writeBuffer , "\n");
            strcat(writeBuffer , "Faculty Name: ");
            strcat(writeBuffer , display_faculty.faculty_name);
            strcat(writeBuffer , "\n");
            strcat(writeBuffer , "Faculty Department: ");
            strcat(writeBuffer , display_faculty.faculty_department);
            strcat(writeBuffer , "\n");
            strcat(writeBuffer , "-----------------------------------");
            strcat(writeBuffer , "\n");
        }
    } else {
        while(read(faculty_fd , &display_faculty , sizeof(struct Faculty)) == sizeof(struct Faculty))
        {
            if(strcmp(readBuffer,display_faculty.faculty_id) == 0)
            {
                strcat(writeBuffer , "-----------------------------------\n");
                strcat(writeBuffer , " Faculty id: ");
                strcat(writeBuffer , display_faculty.faculty_id);
                strcat(writeBuffer , "\n");
                strcat(writeBuffer , "Faculty Name: ");
                strcat(writeBuffer , display_faculty.faculty_name);
                strcat(writeBuffer , "\n");
                strcat(writeBuffer , "Faculty Department: ");
                strcat(writeBuffer , display_faculty.faculty_department);
                strcat(writeBuffer , "\n");
                strcat(writeBuffer , "-----------------------------------");
                strcat(writeBuffer , "\n");
            }
        }
    }
    write(client_FD,writeBuffer,strlen(writeBuffer));
    close(faculty_fd);
    
}
void modify_faculty(int client_FD)
{
    struct Faculty modify_faculty;
    memset(readBuffer, 0, sizeof(readBuffer));
    int faculty_fd = open("faculty_data.txt" , O_RDWR , 0644); //open file
    if(write(client_FD,MODIFY_DETAILS, strlen(MODIFY_DETAILS)) == -1) // data send to client
        perror("modify student loginid write function error");
    //read loginid provided by user through client interface
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1) // read input from client
        perror("modify student loginid read function error");
    char offset[10]; // char array
    strcpy(offset , readBuffer); //readbuffer data copy to offset
        //moving to start of file
    lseek(faculty_fd , 0 , SEEK_SET);
    memset(writeBuffer, 0, sizeof(writeBuffer));
    //bytes need to be left from start
    int bytes = (atoi(offset) - 1) * sizeof(struct Faculty);
    //pointing file cursor to start of record need to be modified
    lseek(faculty_fd,bytes,SEEK_SET);
    read(faculty_fd, &modify_faculty, sizeof(struct Faculty));
    printf("ask user to give input\n");
    do {
    strcat(writeBuffer , "Which information you want to modify => name/department: ");
    if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
        perror("modify faculty write function error");
    memset(readBuffer, 0, sizeof(readBuffer));
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
        perror("modify faculty read function error");
   
    memset(writeBuffer, 0, sizeof(writeBuffer));
        if(strcmp(readBuffer,"name") == 0)
        {
            strcat(writeBuffer , "Faculty name: ");
            strcat(writeBuffer, modify_faculty.faculty_name);
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, "Update faculty name: ");
            if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
                perror("modify faculty name write function error");
            memset(readBuffer, 0, sizeof(readBuffer));
            if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
                perror("modify faculty name read function error");
            //file has come to end of the record moving back to position(start of modified record)
            //lseek(student_fd , -(bytes),SEEK_CUR);
            strcpy(modify_faculty.faculty_name , readBuffer);
            //write(student_fd, &modify_stud, sizeof(struct student));
            printf("faculty name has been updated successfully\n");
            memset(writeBuffer, 0, sizeof(writeBuffer));
            memset(readBuffer, 0, sizeof(readBuffer));
            //lseek(student_fd, -(sizeof(struct student)) , SEEK_CUR);
            //write(student_fd, &modify_stud, sizeof(struct student));
            strcat(writeBuffer, "faculty name has been updated successfully\n");
            strcat(writeBuffer, "\n");
            strcat(writeBuffer,"Do you want to modify more => yes/no\n");
            if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
                perror("ask again modify write function error");
            if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
                perror("ask again modify  read function error");
            
        }
        else if(strcmp(readBuffer,"department") == 0)
        {
            strcat(writeBuffer , "Faculty department: ");
            strcat(writeBuffer, modify_faculty.faculty_department);
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, "Update faculty department: ");
            if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
                perror("modify faculty department write function error");
            memset(readBuffer, 0, sizeof(readBuffer));
            if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
                perror("modify faculty department read function error");
            //file has come to end of the record moving back to position(start of modified record)
            //lseek(student_fd , -(bytes),SEEK_CUR);
            strcpy(modify_faculty.faculty_department , readBuffer);
            //write(student_fd, &modify_stud, sizeof(struct student));
            printf("faculty department has been updated successfully\n");
            memset(writeBuffer, 0, sizeof(writeBuffer));
            memset(readBuffer, 0, sizeof(readBuffer));
            //lseek(student_fd, -(sizeof(struct student)) , SEEK_CUR);
            //write(student_fd, &modify_stud, sizeof(struct student));
            strcat(writeBuffer, "faculty department has been updated successfully\n");
            strcat(writeBuffer, "\n");
            strcat(writeBuffer,"Do you want to modify more => yes/no\n");
            if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
                perror("ask again modify write function error");
            if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
                perror("ask again modify  read function error");
        }
        else
        {
                  memset(writeBuffer, 0, sizeof(writeBuffer));
                  memset(readBuffer, 0, sizeof(readBuffer));
                  strcat(writeBuffer, "You have given wrong input\n");
                  strcat(writeBuffer, "\n");
                  strcat(writeBuffer,"Do you want to modify more => yes/no\n");
                  if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
                      perror("ask again modify write function error");
                  if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
                      perror("ask again modify  read function error");
          
        }
    }while(strcmp(readBuffer , "yes") == 0);
    lseek(faculty_fd, -sizeof(struct student), SEEK_CUR);
    memset(writeBuffer, 0, sizeof(writeBuffer));
    write(faculty_fd,&modify_faculty, sizeof(struct Faculty));
    close(faculty_fd);
}
