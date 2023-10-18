
#include "server_macro.h"


#include "faculty_admin_menu.h"

char readBuff[50] , writeBuff[100];
void add_new_course(int client_FD)

 void add_new_course(int client_FD)
{
     struct Course add_course;
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
     strcat(add_course.fid = login_id);
     strcat(add_course.fname = faculty_name );
     lseek(course_fd, 0, SEEK_END );
     printf("course_db\n");
     write(course_fd, &add_course, sizeof(struct Course));
     close(course_fd);
 }
// view offer course
/*void view_offered_course(int client_FD)
{
    struct Course view_course;
    int course_fd = open("course_data.txt", O_RDONLY, 0644);
    if (course_fd == -1) {
        perror("Error opening the file");
        // Handle the error, maybe exit the function or the program
        return;
    }
    if(write(course_FD , COURSE_DISPLAY , strlen(COURSE_DISPLAY)) == -1) {
        perror("Write system call error display function\n");
    }
    memset(readBuff, 0, sizeof(readBuff));
    memset(writeBuff, 0, sizeof(writeBuff));
    if(read(client_FD,readBuff,sizeof(readBuff)) == -1) {
    perror("read error: view course");
    }
    lseek(course_fd , 0 , SEEK_SET);
    // read each student data and print
    if(strcmp(readBuff,"all") == 0) {
        while(read(course_fd , &view_course , sizeof(struct Course)) !=0 )
        {
            if(strcmp(view_course.fid , login_id) == 0)
            {
                strcat(writeBuff , "-----------------------------------\n");
                strcat(writeBuff , "Course id: ");
                strcat(writeBuff , view_course.cid);
                strcat(writeBuff , "\n");
                strcat(writeBuff , "Course Name: ");
                strcat(writeBuff , view_course.cname);
                strcat(writeBuff , "\n");
                strcat(writeBuff , "Course Credit: ");
                strcat(writeBuff , view_course.credit);
                strcat(writeBuff , "\n");
                strcat(writeBuff , "Course Faculty Id: ");
                strcat(writeBuff , view_course.fid);
                strcat(writeBuff , "\n");
                strcat(writeBuff , "Course Faculty Name: ");
                strcat(writeBuff , view_course.fname);
                strcat(writeBuff , "-----------------------------------\n");
            }
        }
    }
     else {
         while(read(course_fd , &view_course , sizeof(struct Course)) !=0 )
         {
        if(strcmp(view_course.fid , login_id) == 0 && strcmp(view_course.cid , readBuff) == 0)
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
        strcat(writeBuff , "Course Faculty Name: ");
        strcat(writeBuff , view_course.fname);
        strcat(writeBuff , "-----------------------------------\n");
        strcat(writeBuff , "press enter to continue: \n");
        }
    }
    write(client_FD,writeBuff,strlen(writeBuff));
    close(course_fd);
    
}
// view_course_enrollment_status
void view_course_enrollment_status(int client_FD){
    struct Course view_enroll;
    int course_fd = open("course_data.txt", O_RDONLY, 0644);
    memset(writeBuff, 0, sizeof(writeBuff));
    memset(readBuff, 0, sizeof(readBuff));
    write(client_FD , COURSE_ID , strlen(COURSE_ID));
    read(client_FD , readBuff , sizeof(readBuff));
    while(read(course_fd , &view_enroll , sizeof(struct Course)) !=0 )
    {
    if(strcmp(view_course.fid , login_id) == 0 && strcmp(view_course.cid , readBuff) == 0){
       strcat(writeBuff , "-----------------------------------\n");
       strcat(writeBuff , "Course Name: ");
       strcat(writeBuff , view_enroll.cname);
       strcat(writeBuff , "\n");
       strcat(writeBuff , "Course Credit: ");
       strcat(writeBuff , view_enroll.credit);
       strcat(writeBuff , "\n");
       strcat(writeBuff , "Total Seats: ");
       strcat(writeBuff , view_enroll.total);
       strcat(writeBuff , "\n");
       strcat(writeBuff , "Occupied Seats ");
       strcat(writeBuff , view_enroll.occupied);
       strcat(writeBuff , "Available Seats ");
       strcat(writeBuff , view_enroll.available);
       strcat(writeBuff , "-----------------------------------\n");
       strcat(writeBuff , "press enter to continue: \n");
    }
  }
}
//remove course
void remove_course(int client_FD) {
    struct Course removec;
    int fd = open(course_data.txt, O_RDONLY);
    int tempFd = open(course_data_tmp.txt, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    memset(writeBuff, 0, sizeof(writeBuff));
    memset(readBuff, 0, sizeof(readBuff));
    write(client_FD , REMV_C , strlen(REMV_C));
    lseek(fd , 0 ,SEEK_SET);
    lseek(fd , 0 ,SEEK_SET);
    read(client_FD , readBuff , &readBuff);
    while(read(fd , &removec , sizeof(struct Course)) > 0 )
    {
        
        if(strcmp(removec.cid , readBuff) !=  0)
        {
            write(tempFd , &removec , sizeof(struct Course));
        }
        
    }
    
    if (rename(course_data_tmp.txt, course_data.txt) == -1) {
        perror("error while renaming");
    }
    
}*/








