#include "server_macro.h"
#include "./file_data/student.h"
#include "./file_data/course.h"
#include "./file_data/enroll.h"
#include<fcntl.h>
#include<string.h>
#include<stdbool.h>
void view_course(int client_FD);
void student_menu(int client_FD);
void enroll_course(int client_FD);
void unenroll_course(int client_FD);

char readBuf[100] , writeBuf[1000];
int loginid;
struct student credstud;
bool login_Credentials(int client_FD, char login_id[5])
{
    int student_FD = open("Student_DB.txt", O_RDONLY);
    lseek(student_FD,0,SEEK_SET);
    int loginid = atoi(login_id);
     off_t offset = lseek(student_FD,(loginid-1) * sizeof(struct student), SEEK_SET);
     size_t readBytes = read(student_FD , &credstud,sizeof(struct student));
     if(readBytes > 0)
     {
     char password[20];
     strcpy(password, credstud.pwd);
     printf("password by struct: %s",password);
     if(write(client_FD , STUDENT_PWD , strlen(STUDENT_PWD)) == -1)
            perror("Write system call error\n");
        memset(readBuf, 0, sizeof(readBuf));
     read(client_FD, readBuf , sizeof(readBuf));
     if(strcmp(password , readBuf) == 0)
        return true;
    else
        return false;
     }
     return false;
}
void student_menu(int client_FD)
{
    if(write(client_FD , STUDENT_LOGIN , strlen(STUDENT_LOGIN)) == -1)
        perror("Write system call error\n");
    read(client_FD, readBuf , sizeof(readBuf));
    bool cred_check = login_Credentials(client_FD,readBuf);
    if(cred_check)
    {
         printf("Bach to student menu");
        if(write(client_FD , STUDENT_MENU , strlen(STUDENT_MENU)) == -1)
            perror("write student menu error\n");
        memset(readBuf, 0, sizeof(readBuf));
        if(read(client_FD , readBuf , sizeof(readBuf)) == -1)
            perror("read student choice error\n");
        int student_choice = atoi(readBuf);
        switch(student_choice)
        {
            case 1: 
                view_course(client_FD);
                break;
            case 2: 
                enroll_course(client_FD);
                break;
            case 3:
                unenroll_course(client_FD);
            default:
                break;
        }
    }
    else {
        write(client_FD , STUDENT_WRONG_CRED , strlen(STUDENT_WRONG_CRED));
        exit(0);
        
    }
}
    void view_course(int client_FD) {
        char str[20];
        char str1[20];
        char str2[20];
        struct flock lock;
         lock.l_type = F_RDLCK;
         lock.l_whence = SEEK_SET;
         lock.l_start = 0;
         lock.l_len = 0; // Lock the entire file
        int course_fd = open("course_data.txt",O_RDONLY);
        struct Course view_course;
         memset(writeBuf, 0, sizeof(writeBuf)); // to clear writeBuffer content
         memset(readBuf, 0, sizeof(readBuf));
         strcat(writeBuf , "Type 1 to view all courses\n");
         //strcat(writeBuf , "Read Lock Applied\n");
         write(client_FD , writeBuf,strlen(writeBuf));
         read(client_FD,readBuf,sizeof(readBuf));
         if(strcmp(readBuf,"1") == 0)
    {       
        //Apply read lock
         fcntl(course_fd, F_SETLKW, &lock); 
            while(read(course_fd , &view_course , sizeof(struct Course)) > 0 )
        {
            printf("inside while\n");
                strcat(writeBuf,"Read lock applied\n");
                strcat(writeBuf , "-----------------------------------\n");
                strcat(writeBuf , "Course Id: \t");
                strcat(writeBuf , view_course.cid);
                strcat(writeBuf , "\n");
                strcat(writeBuf , "Course Name: \t");
                strcat(writeBuf , view_course.cname);
                strcat(writeBuf , "\n");
                strcat(writeBuf , "Course Credit: \t");
                strcat(writeBuf , view_course.credit);
                strcat(writeBuf , "\n");
                strcat(writeBuf , "Course Faculty Id: \t");
                strcat(writeBuf , view_course.fid);
                strcat(writeBuf , "\n");
                strcat(writeBuf , "Course Faculty Name: \t");
                strcat(writeBuf , view_course.fname);
                strcat(writeBuf , "\n");
                strcat(writeBuf , "Total Seats: \t");
                sprintf(str, "%d", view_course.total_seats);
                strcat(writeBuf , str);
                strcat(writeBuf , "\n");
                strcat(writeBuf , "Available Seats: \t");
                sprintf(str1, "%d", view_course.available_seats);
                strcat(writeBuf , str1);
                strcat(writeBuf , "\n");
                strcat(writeBuf , "Occupied Seats: \t");
                sprintf(str2, "%d", view_course.occupied_seats);
                strcat(writeBuf , str2);
                strcat(writeBuf , "\n");
                strcat(writeBuf , "-----------------------------------\n");
                printf("outside while\n");
      }
    }
            strcat(writeBuf ,"press 0 to unlock:\t");
             printf("write course student details to client\n");
           if(write(client_FD,writeBuf,strlen(writeBuf)) == -1)
           perror("error in write");
           read(client_FD , readBuf , strlen(readBuf));
           //apply unlock
          lock.l_type = F_UNLCK;
          fcntl(course_fd, F_SETLK, &lock);
            printf("file unlocked\n");
            close(course_fd);

}
void enroll_course(int client_FD)
{
    struct Enroll en;
    struct Course cs;
    char sid[10];
     struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int enroll_fd = open("enroll.txt" , O_RDWR | O_CREAT, 0644);
    int course_fd = open("course_data.txt" , O_RDWR , 0644);
    lseek(course_fd,0,SEEK_SET); // bring to start
    memset(writeBuf, 0, sizeof(writeBuf)); // to clear writeBuffer content
    memset(readBuf, 0, sizeof(readBuf));
    strcat(writeBuf , "Enter the couse id to enroll in:\t");
    if(write(client_FD , writeBuf, strlen(writeBuf)) == -1)
        perror("write enroll error");
    if(read(client_FD , readBuf, sizeof(readBuf)) == -1)
        perror("read enroll error");
    int FLAG = 0;
    int count = 0;
    //Apply read lock
    fcntl(course_fd, F_SETLKW, &lock);
    while(read(course_fd,&cs,sizeof(struct Course))!=0)
    {
        count++;
        if(strcmp(cs.cid , readBuf) == 0) {
        FLAG = 1;
        break;
        }
    }
    //printf("count" , count);
    //apply unlock
    lock.l_type = F_UNLCK;
    fcntl(course_fd, F_SETLK, &lock);
    if(FLAG == 1) {
        lseek(enroll_fd, 0, SEEK_END );
        snprintf(en.sid, sizeof(en.sid), "%d",loginid);
        strcpy(en.cid, readBuf);
        write(enroll_fd, &en , sizeof(struct Enroll));
        lseek(course_fd , 0 , SEEK_SET);
        lock.l_type = F_WRLCK;
        lock.l_start = (count-1)*sizeof(struct Course);
        lock.l_len = sizeof(struct Course);
        fcntl(course_fd, F_SETLKW, &lock);
        --(cs.available_seats);
        --(cs.total_seats);
        ++(cs.occupied_seats);
        lock.l_type = F_UNLCK;
        fcntl(course_fd, F_SETLK, &lock);
     }
    else{
           memset(writeBuf, 0, sizeof(writeBuf));
            strcat(writeBuf,"No such course_id exist\n");
            strcat(writeBuf,"Press any key to exit\n");
            write(client_FD,writeBuf,strlen(writeBuf));
            read(client_FD,readBuf,sizeof(readBuf));
            memset(writeBuf, 0, sizeof(writeBuf));
            strcpy(writeBuf , "end");
            write(client_FD,writeBuf,strlen(writeBuf));
    }
    memset(writeBuf, 0, sizeof(writeBuf));
    strcat(writeBuf,"Course Enrolled Successfully\n");
    strcat(writeBuf,"Press any key to exit\n");
    write(client_FD,writeBuf,strlen(writeBuf));
    read(client_FD,readBuf,sizeof(readBuf));
    memset(writeBuf, 0, sizeof(writeBuf));
    memset(writeBuf, 0, sizeof(writeBuf));
    strcpy(writeBuf , "end");
    write(client_FD,writeBuf,strlen(writeBuf));
    close(enroll_fd);
    close(course_fd);
}

void unenroll_course(int client_FD) {
    struct Enroll ec;
    int fd = open("enroll.txt", O_RDONLY);
    int tempFd = open("enroll_tmp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    memset(writeBuf, 0, sizeof(writeBuf)); // to clear writeBuffer content
    memset(readBuf, 0, sizeof(readBuf));
    strcat(writeBuf , "Enter the couse id to enroll in:\t");
    if(write(client_FD , writeBuf, strlen(writeBuf)) == -1)
        perror("write enroll error");
    if(read(client_FD , readBuf, sizeof(readBuf)) == -1)
        perror("read enroll error");
    char enroll_sid[10];
    snprintf(enroll_sid, sizeof(enroll_sid), "%d",loginid);
    memset(writeBuf, 0, sizeof(writeBuf));
    memset(readBuf, 0, sizeof(readBuf));
    //write(client_FD , REMV_C , strlen(REMV_C));
    lseek(fd , 0 ,SEEK_SET);
    //lseek(fd , 0 ,SEEK_SET);
    //read(client_FD , readBuf , &readBuf);
    while(read(fd , &ec , sizeof(struct Enroll)) > 0 )
    {
        
        if(strcmp(ec.cid , readBuf) !=  0 && strcmp(ec.sid,enroll_sid) !=0)
        {
            write(tempFd , &ec , sizeof(struct Enroll));
        }
        
    }
    
    if (rename("enroll_tmp.txt", "enroll.txt") == -1) {
        perror("error while renaming");
    }
}



