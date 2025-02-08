#ifndef CLIENT_H
#define CLIENT_H
#include "message.h"

void init_socket(const char *server_ip, int server_port);  
void close_socket();                                      
void send_message(Message *msg);   

void add_student(int roll_no, const char *name, float cgpa, int no_of_courses);
void add_course(int roll_no, int course_code, int marks);
void modify_student(int roll_no, float new_cgpa);
void modify_course(int roll_no, int course_code, int new_marks);
void delete_student(int roll_no);
void delete_course(int roll_no, int course_code);

#endif 