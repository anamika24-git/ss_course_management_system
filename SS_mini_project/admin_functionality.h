#include "server_macro.h"
#include "./file_data/student.h"
#include<fcntl.h>
#include<string.h>



void admin_menu(int client_FD);
void add_student(int client_FD);
void view_student_details(int client_FD);
void modify_student(int client_FD);
char writeBuffer[1500] , readBuffer[50];
struct student stud;
void admin_menu(int client_FD)
{
    int admin_choice;
    do{
        printf("back to pavilion\n");
    if(write(client_FD , ADMIN_MENU , strlen(ADMIN_MENU)) == -1) {
        perror("Write system call error\n");
    }
        memset(readBuffer, 0, sizeof(readBuffer));
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
    {
        perror("read system call error : while reading user input from client\n");
    }
    admin_choice = atoi(readBuffer);

        switch(admin_choice)
        {
            case 1:
                 add_student(client_FD);
                printf("I am back to case 1\n");
                memset(writeBuffer, 0, sizeof(writeBuffer));
                printf("middel0\n");
                strcat(writeBuffer , "Student added successfully\n");
                printf("middle1\n");
                strcat(writeBuffer , "Student initial password is: ");
                printf("middle2\n");
                strcat(writeBuffer, stud.pwd);
                printf("middle3\n");
                strcat(writeBuffer, "\n");
                printf("middle4\n");
                strcat(writeBuffer , "Student login id is: ");
                printf("middle5\n");
                strcat(writeBuffer, stud.loginid);
                printf("middle6\n");
                strcat(writeBuffer, "\n");
                printf("done \n");
                break;
            case 2:
                view_student_details(client_FD);
                printf("back to case 2\n");
                memset(writeBuffer, 0, sizeof(writeBuffer));
                break;
            case 3:
                modify_student(client_FD);
                printf("back to case 3\n");
                memset(writeBuffer, 0, sizeof(writeBuffer));
                break;
            default:
                printf("add menu later\n");
                exit(0);
        }
        printf("continue or not\n");
        strcat(writeBuffer, "Do you wish to operate admin menu again: yes/no: \n");
        printf("write case out1\n");
        if(write(client_FD , writeBuffer , strlen(writeBuffer)) == -1)
            perror("write error");
        printf("write case out2\n");
        memset(readBuffer, 0, sizeof(readBuffer));
        printf("write case out3\n");
        if(read(client_FD , readBuffer , sizeof(readBuffer))==-1)
            perror("read error");
        printf("last line\n");
    } while(strcmp(readBuffer , "yes") == 0);

}

void add_student(int client_FD)
{
    
    int student_fd = open("student_DB.txt", O_RDWR | O_CREAT, 0644);

    if (student_fd == -1) {
        perror("Error opening the file");
        // Handle the error, maybe exit the function or the program
        return;
    }
    //adding student roll number
    memset(writeBuffer, 0, sizeof(writeBuffer)); // to clear writeBuffer content
    memset(readBuffer, 0, sizeof(readBuffer));  //to clear readbuffer content
    if(write(client_FD,STUDENT_ROLL, strlen(STUDENT_ROLL)) == -1)
        perror("Issue on writing student roll number on client side");
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
        perror("Issue on reading student roll number from client side");
    else {
        printf("Received student roll number: %s\n", readBuffer);
        strcpy(stud.roll,readBuffer);
    }
    
    //adding student name
    memset(writeBuffer, 0, sizeof(writeBuffer)); // to clear writeBuffer content
    memset(readBuffer, 0, sizeof(readBuffer));  //to clear readbuffer content
    if(write(client_FD,STUDENT_NAME, strlen(STUDENT_NAME)) == -1)
        perror("Issue on writing student roll number on client side");
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
        perror("Issue on reading student name  from client side");
    
    else{
        printf("Received student name: %s\n", readBuffer);
        strcpy(stud.name , readBuffer);
    }
    
    //adding student age
    memset(writeBuffer, 0, sizeof(writeBuffer)); // to clear writeBuffer content
    memset(readBuffer, 0, sizeof(readBuffer));  //to clear readbuffer content
    if(write(client_FD,STUDENT_AGE, strlen(STUDENT_AGE)) == -1)
        perror("Issue on writing student age on client side");
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
        perror("Issue on reading student age  from client side");
    else
    {
        printf("Received student age: %s\n", readBuffer);
        strcpy(stud.age , readBuffer);
    }
    
    //adding student location
    memset(writeBuffer, 0, sizeof(writeBuffer)); // to clear writeBuffer content
    memset(readBuffer, 0, sizeof(readBuffer));  //to clear readbuffer content;
    if(write(client_FD,STUDENT_LOCATION, strlen(STUDENT_LOCATION)) == -1)
        perror("Issue on writing student location on client side");
    
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
        perror("Issue on reading student location  from client side");
    printf("Received student location: %s\n", readBuffer);
    printf("here1 \n");
    strcpy(stud.location, readBuffer);
    strcpy(stud.pwd, stud.name);
    strcpy(stud.pwd, stud.age);
    //go to end
    int data;
    struct student temp;
    //Check if it's the first record (based on the file size)
    if (lseek(student_fd, -1*(sizeof(struct student)), SEEK_END ) == -1)
        data = 1;// Set an initial value for loginid
    else {
        
        read(student_fd, &temp, sizeof(struct student));
        data = atoi(temp.loginid);
        // Increment the integer
        ++data;
        
        //lseek(student_fd, 0, SEEK_END);
    }
        // Move file pointer to the end of the file
        
        // Convert the incremented integer back to string and store it in loginid
    sprintf(stud.loginid, "%d",data);
    // writing student info in db file
    lseek(student_fd, 0, SEEK_END );
    printf("student_db\n");
    write(student_fd, &stud, sizeof(struct student));
    close(student_fd);
}
    
void view_student_details(int client_FD)
{
    struct student display_student;
    int student_fd = open("student_DB.txt", O_RDONLY, 0644);
    if (student_fd == -1) {
        perror("Error opening the file");
        // Handle the error, maybe exit the function or the program
        return;
    }
    if(write(client_FD , DISPLAY , strlen(DISPLAY)) == -1) {
        perror("Write system call error display function\n");
    }
    memset(readBuffer, 0, sizeof(readBuffer));
    memset(writeBuffer, 0, sizeof(writeBuffer));
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1) {
    perror("read system call error display function: while reading user input from client\n");
    }
    lseek(student_fd , 0 , SEEK_SET);
    // read each student data and print
    if(strcmp(readBuffer,"every") == 0) {
        while(read(student_fd , &display_student , sizeof(struct student)) !=0 )
        {
            strcat(writeBuffer , "-----------------------------------\n");
            strcat(writeBuffer , "Student loginid: ");
            strcat(writeBuffer , display_student.loginid);
            strcat(writeBuffer , "\n");
            strcat(writeBuffer , "Student Name: ");
            strcat(writeBuffer , display_student.name);
            strcat(writeBuffer , "\n");
            strcat(writeBuffer , "Student Age: ");
            strcat(writeBuffer , display_student.age);
            strcat(writeBuffer , "\n");
            strcat(writeBuffer , "Student Location: ");
            strcat(writeBuffer , display_student.location);
            strcat(writeBuffer , "\n");
            strcat(writeBuffer , "-----------------------------------\n");
        }
    } else {
        while(read(student_fd , &display_student , sizeof(struct student)) == sizeof(struct student))
        {
            if(strcmp(readBuffer,display_student.loginid) == 0)
            {
                strcat(writeBuffer , "-----------------------------------\n");
                strcat(writeBuffer , "Student login_id: ");
                strcat(writeBuffer , display_student.loginid);
                strcat(writeBuffer , "\n");
                strcat(writeBuffer , "Student Name: ");
                strcat(writeBuffer , display_student.name);
                strcat(writeBuffer , "\n");
                strcat(writeBuffer , "Student Age: ");
                strcat(writeBuffer , display_student.age);
                strcat(writeBuffer , "\n");
                strcat(writeBuffer , "Student Location: ");
                strcat(writeBuffer , display_student.location);
                strcat(writeBuffer , "\n");
                strcat(writeBuffer , "-----------------------------------\n");
            }
        }
    }
    write(client_FD,writeBuffer,strlen(writeBuffer));
    close(student_fd);
    
}
void modify_student(int client_FD)
{
    struct student modify_stud;
    memset(readBuffer, 0, sizeof(readBuffer));
    int student_fd = open("student_DB.txt" , O_RDWR , 0644); //open file
    if(write(client_FD,MODIFY_DETAILS, strlen(MODIFY_DETAILS)) == -1) // data send to client
        perror("modify student loginid write function error");
    //read loginid provided by user through client interface
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1) // read input from client
        perror("modify student loginid read function error");
    char offset[10]; // char array
    strcpy(offset , readBuffer); //readbuffer data copy to offset
        //moving to start of file
    lseek(student_fd , 0 , SEEK_SET);
    memset(writeBuffer, 0, sizeof(writeBuffer));
    //bytes need to be left from start
    int bytes = (atoi(offset) - 1) * sizeof(struct student);
    //pointing file cursor to start of record need to be modified
    lseek(student_fd,bytes,SEEK_SET);
    read(student_fd, &modify_stud, sizeof(struct student));
    printf("ask user to give input\n");
    do {
    strcat(writeBuffer , "Which information you want to modify => name/age/location: ");
    if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
        perror("modify student write function error");
    memset(readBuffer, 0, sizeof(readBuffer));
    if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
        perror("modify student read function error");
   
    memset(writeBuffer, 0, sizeof(writeBuffer));
        if(strcmp(readBuffer,"name") == 0)
        {
            strcat(writeBuffer , "Student name: ");
            strcat(writeBuffer, modify_stud.name);
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, "Update student name: ");
            if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
                perror("modify student name write function error");
            memset(readBuffer, 0, sizeof(readBuffer));
            if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
                perror("modify student name read function error");
            //file has come to end of the record moving back to position(start of modified record)
            //lseek(student_fd , -(bytes),SEEK_CUR);
            strcpy(modify_stud.name , readBuffer);
            //write(student_fd, &modify_stud, sizeof(struct student));
            printf("student name has been updated successfully\n");
            memset(writeBuffer, 0, sizeof(writeBuffer));
            memset(readBuffer, 0, sizeof(readBuffer));
            //lseek(student_fd, -(sizeof(struct student)) , SEEK_CUR);
            //write(student_fd, &modify_stud, sizeof(struct student));
            strcat(writeBuffer, "student name has been updated successfully\n");
            strcat(writeBuffer, "\n");
            strcat(writeBuffer,"Do you want to modify more => yes/no\n");
            if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
                perror("ask again modify write function error");
            if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
                perror("ask again modify  read function error");
            
        }
        else if(strcmp(readBuffer,"age") == 0)
        {
            strcat(writeBuffer , "Student age: ");
            strcat(writeBuffer, modify_stud.age);
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, "Update student age: ");
            if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
                perror("modify student age write function error");
            memset(readBuffer, 0, sizeof(readBuffer));
            if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
                perror("modify student age read function error");
            //file has come to end of the record moving back to position(start of modified record)
            //lseek(student_fd , -(bytes),SEEK_CUR);
            strcpy(modify_stud.age , readBuffer);
            //write(student_fd, &modify_stud, sizeof(struct student));
            printf("student age has been updated successfully\n");
            memset(writeBuffer, 0, sizeof(writeBuffer));
            memset(readBuffer, 0, sizeof(readBuffer));
            //lseek(student_fd, -(sizeof(struct student)) , SEEK_CUR);
            //write(student_fd, &modify_stud, sizeof(struct student));
            strcat(writeBuffer, "student age has been updated successfully\n");
            strcat(writeBuffer, "\n");
            strcat(writeBuffer,"Do you want to modify more => yes/no\n");
            if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
                perror("ask again modify write function error");
            if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
                perror("ask again modify  read function error");
        }
        else
        {
            strcat(writeBuffer , "Student location: ");
            strcat(writeBuffer, modify_stud.location);
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, "Update student location: ");
            if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
                perror("modify student location write function error");
            memset(readBuffer, 0, sizeof(readBuffer));
            if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
                perror("modify student location read function error");
            //file has come to end of the record moving back to position(start of modified record)
            //lseek(student_fd , -(bytes),SEEK_CUR);
            strcpy(modify_stud.location , readBuffer);
            //write(student_fd, &modify_stud, sizeof(struct student));
            printf("student loction has been updated successfully\n");
            memset(writeBuffer, 0, sizeof(writeBuffer));
            memset(readBuffer, 0, sizeof(readBuffer));
            //lseek(student_fd, -(sizeof(struct student)) , SEEK_CUR);
            //write(student_fd, &modify_stud, sizeof(struct student));
            strcat(writeBuffer, "student location has been updated successfully\n");
            strcat(writeBuffer, "\n");
            strcat(writeBuffer,"Do you want to modify more => yes/no\n");
            if(write(client_FD,writeBuffer, strlen(writeBuffer)) == -1)
                perror("ask again modify write function error");
            if(read(client_FD,readBuffer,sizeof(readBuffer)) == -1)
                perror("ask again modify read function error");
        }
    }while(strcmp(readBuffer , "yes") == 0);
    lseek(student_fd, -sizeof(struct student), SEEK_CUR);
    memset(writeBuffer, 0, sizeof(writeBuffer));
    write(student_fd,&modify_stud, sizeof(struct student));
    close(student_fd);
    
}

