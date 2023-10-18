
#ifndef COURSE_RECORD
#define COURSE_RECORD

struct Course
{
    char cid[20];
    char cname[20];
    char credit[20];
    char fid[20];
    char fname[20];
    int total_seats;
    int available_seats;
    int occupied_seats;
    
    
};

#endif


