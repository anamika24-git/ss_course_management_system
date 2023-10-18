#include "server_macro.h"
#include "./file_data/faculty.h"
#include "./file_data/course.h"
#include<fcntl.h>
#include<string.h>

//#include "faculty_functionality.h"

char readBuff[50] , writeBuff[2500];
struct Faculty fac;
char login_id[10];
const char *emptyBuffer = "";
void faculty_menu(int client_FD);
void add_new_course(int client_FD);
void view_offered_course(int client_FD);

void faculty_menu(int client_FD)
{
    memset(readBuff, 0, sizeof(readBuff));
    memset(writeBuff, 0, sizeof(writeBuff));
    int faculty_fd = open("faculty_data.txt", O_RDONLY);
    lseek(faculty_fd , 0 ,SEEK_SET);
    if(write(client_FD , FACULTY_LOGIN , strlen(FACULTY_LOGIN)) == -1) {
        perror("Write system call error\n");
    }
    read(client_FD, readBuff , sizeof(readBuff));
     strcpy(login_id,readBuff);
    int flag = 0;
    while(read(faculty_fd , &fac , sizeof(struct Faculty)) > 0 )
    {
        printf("faculty id buff\n: %s",readBuff);
        printf("faculty id struct\n: %s",fac.faculty_id);
        
        if(strcmp(fac.faculty_id, readBuff) == 0)
        {
            flag = 1;
            write(client_FD , FACULTY_PWD, strlen(FACULTY_PWD));
            memset(readBuff, 0, sizeof(readBuff));
            read(client_FD, readBuff , sizeof(readBuff));
            printf("1\n");
            printf("faculty password buff: %s\n",readBuff);
            printf("faculty password struct: %s\n",fac.faculty_pwd);
            if(strcmp(fac.faculty_pwd , readBuff) == 0)
            {
                printf("2\n");
                printf("faculty password buff: %s\n",readBuff);
                printf("faculty password struct: %s\n",fac.faculty_pwd);
                memset(writeBuff, 0, sizeof(writeBuff));
                strcat(writeBuff , "Logged in successfully \n");
                do {
                strcat(writeBuff ,"choose your type\n");
                strcat(writeBuff ,"1. Add course\n");
                strcat(writeBuff ,"2. view course\n");
                printf("write\n");
                if(write(client_FD , writeBuff, strlen(writeBuff)) == -1) {
                    perror("Write system call error\n");
                }
                memset(readBuff, 0, sizeof(readBuff));
                if(read(client_FD,readBuff,sizeof(readBuff)) == -1)
                {
                    perror("read system call error : while reading user input from client\n");
                }
                int faculty_choice = atoi(readBuff);
                
                switch(faculty_choice)
                {
                    case 1:
                        add_new_course(client_FD);
                        printf("I am back to case 1\n");
                        memset(writeBuff, 0, sizeof(writeBuff));
                        printf("middel0\n");
                        strcat(writeBuff , "Course Added Successfully\n");
                        break;
                    case 2:
                        view_offered_course(client_FD);
                        printf("I am back to case 2\n");
                        memset(writeBuff, 0, sizeof(writeBuff));
                        break;
                    default:
                        write(client_FD, emptyBuffer, 0);
                        
                }
                    printf("continue or not\n");
                    strcat(writeBuff, "Do you wish to operate admin menu again: yes/no: \n");
                    printf("login_id continue: %s\n",fac.faculty_id);
                    if(write(client_FD , writeBuff , strlen(writeBuff)) == -1)
                        perror("write error");
                     memset(readBuff, 0, sizeof(readBuff));
                    if(read(client_FD , readBuff , sizeof(readBuff))==-1)
                    printf("last line\n");
                    memset(writeBuff, 0, sizeof(writeBuff));
                }while(strcmp(readBuff , "yes") == 0);
                break;
                } // password if close
               else {
                memset(writeBuff, 0, sizeof(writeBuff));
                strcat(writeBuff , "Wrong Password\nPress enter to exit\n");
                write(client_FD , writeBuff , strlen(writeBuff));
                break; //out of while loop
            }
            
        } // login if close
        
    } // while close
    if(flag == 0){
        memset(writeBuff, 0, sizeof(writeBuff));
        memset(readBuff, 0, sizeof(readBuff));
        strcat(writeBuff , "Login Id Not Found\nPress any key to exit");
        write(client_FD , writeBuff , strlen(writeBuff));
        read(client_FD , readBuff,sizeof(readBuff));
    }
   // free(login_id);
    write(client_FD, emptyBuffer, 0);
    }
    
    // add new course
    void add_new_course(int client_FD)
   {
        struct Course add_course;
        printf("1 => LOGIN_ID_ADD COURSE %s\n",login_id);
        int course_fd = open("course_data.txt", O_RDWR | O_CREAT, 0644);
        //ADD COURSE DETAILS
        memset(writeBuff, 0, sizeof(writeBuff)); // to clear writeBuff content
        memset(readBuff, 0, sizeof(readBuff));  //to clear readBuff content
        write(client_FD,COURSE_ID, strlen(COURSE_ID));
        if(read(client_FD,readBuff,sizeof(readBuff)) == -1)
            perror("Issue on reading course id from client side");
        else {
            printf("Received course id: %s\n", readBuff);
            strcpy(add_course.cid,readBuff);
        }
        memset(writeBuff, 0, sizeof(writeBuff)); // to clear writeBuff content
        memset(readBuff, 0, sizeof(readBuff));  //to clear readBuff content
        write(client_FD,COURSE_NAME, strlen(COURSE_NAME));
        if(read(client_FD,readBuff,sizeof(readBuff)) == -1)
            perror("Issue on reading course name from client side");
        else {
            printf("Received couse name: %s\n", readBuff);
            strcpy(add_course.cname,readBuff);
        }
        memset(writeBuff, 0, sizeof(writeBuff)); // to clear writeBuff content
        memset(readBuff, 0, sizeof(readBuff));  //to clear readBuff content
        write(client_FD,COURSE_CREDIT, strlen(COURSE_CREDIT));
        if(read(client_FD,readBuff,sizeof(readBuff)) == -1)
            perror("Issue on reading course credit from client side");
        else {
            printf("Received couse credit: %s\n", readBuff);
            strcpy(add_course.credit,readBuff);
        }
        strcpy(add_course.fid, login_id);
        // add faculty name
        memset(writeBuff, 0, sizeof(writeBuff)); // to clear writeBuff content
        memset(readBuff, 0, sizeof(readBuff));  //to clear readBuff content
        write(client_FD,COURSE_FNAME, strlen(COURSE_FNAME));
        if(read(client_FD,readBuff,sizeof(readBuff)) == -1)
            perror("Issue on reading course credit from client side");
        else {
            printf("Received couse credit: %s\n", readBuff);
            strcpy(add_course.fname,readBuff);
        }
        //add total seats
        memset(writeBuff, 0, sizeof(writeBuff)); // to clear writeBuff content
        memset(readBuff, 0, sizeof(readBuff));  //to clear readBuff content
        write(client_FD,COURSE_TOTALSEAT, strlen(COURSE_TOTALSEAT));
        if(read(client_FD,readBuff,sizeof(readBuff)) == -1)
            perror("Issue on reading course credit from client side");
        else {
            printf("Received couse credit: %s\n", readBuff);
            add_course.total_seats = atoi(readBuff);
        }
        add_course.available_seats = atoi(readBuff);
        add_course.occupied_seats = 0;
        lseek(course_fd, 0, SEEK_END );
        printf("course_db\n");
        write(course_fd, &add_course, sizeof(struct Course));
        close(course_fd);
        printf("1 => LOGIN_ID_ADD COURSE %s\n",login_id);
    }
void view_offered_course(int client_FD)
{
    printf("1 => LOGIN_ID_View course %s\n",login_id);
    struct Course view_course;
    int course_fd = open("course_data.txt", O_RDONLY);
    if (course_fd == -1) {
        perror("Error opening the file");
        // Handle the error, maybe exit the function or the program
        return;
    }
    if(write(client_FD , COURSE_DISPLAY , strlen(COURSE_DISPLAY)) == -1) {
        perror("Write system call error display function\n");
    }
    memset(readBuff, 0, sizeof(readBuff));
    memset(writeBuff, 0, sizeof(writeBuff));
    if(read(client_FD,readBuff,sizeof(readBuff)) == -1) {
    perror("read error: view course");
    }
    lseek(course_fd , 0 , SEEK_SET);
    // read each student data and print
    printf("check 1\n");
    if(strcmp(readBuff,"ALL") == 0) {
        printf("1.login_id: %s\n",login_id);
        while(read(course_fd , &view_course , sizeof(struct Course)) != 0 )
        {
            printf("go inside while loop of if of view course /n");
            printf("login_id: %s p\n",login_id);
            printf("course fid: %s p\n",view_course.fid);
            printf("course cid: %s p\n",view_course.cid);
            printf("course name: %s p\n",view_course.cname);
            //if(strcmp(login_id , view_course.fid) == 0)
           //{
                printf("check 2\n");
                //memset(writeBuff, 0, sizeof(writeBuff));
                strcat(writeBuff , "-----------------------------------\n");
                strcat(writeBuff , "Course id: ");
                strcat(writeBuff , view_course.cid);
                strcat(writeBuff , "\n");
                strcat(writeBuff , "Course Name: ");
                strcat(writeBuff , view_course.cname);
                strcat(writeBuff , "\n");
                /*strcat(writeBuff , "Course Credit: ");
                strcat(writeBuff , view_course.credit);
                strcat(writeBuff , "\n");
                strcat(writeBuff , "Course Faculty Id: ");
                strcat(writeBuff , view_course.fid);
                strcat(writeBuff , "\n");
                //strcat(writeBuff , "Course Faculty Name: ");
                //strcat(writeBuff , view_course.fname);*/
                strcat(writeBuff , "-----------------------------------\n");
               printf("out of if\n");
            //}
            printf("wiew course write\n");
           // write(client_FD,writeBuff,strlen(writeBuff));
            printf("check 4\n");
        }
    }
    else {
        while(read(course_fd , &view_course , sizeof(struct Course)) !=0 )
        {
            printf("go inside while loop of else of view course");
            int login = atoi(login_id);
            int fid = atoi(view_course.fid);
            if( fid == login && strcmp(view_course.cid , readBuff) == 0)
            {
                strcat(writeBuff , "-----------------------------------\n");
                strcat(writeBuff , "Course Name: ");
                strcat(writeBuff , view_course.cname);
                strcat(writeBuff , "\n");
                strcat(writeBuff , "Course Credit: ");
                strcat(writeBuff , view_course.credit);
                strcat(writeBuff , "\n");
                strcat(writeBuff , "Course Faculty Id: ");
                strcat(writeBuff , view_course.fid);
                strcat(writeBuff , "\n");
                strcat(writeBuff , login_id);
                strcat(writeBuff , "\n");                // strcat(writeBuff , "Course Faculty Name: ");
                //strcat(writeBuff , view_course.fname);
                strcat(writeBuff , "-----------------------------------\n");
                strcat(writeBuff , "press enter to continue: \n");
                break;
            }
        }// loop
    }
    printf("write course details to client");
   if(write(client_FD,writeBuff,strlen(writeBuff)) == -1)
       perror("error in write");
    close(course_fd);
    
}

