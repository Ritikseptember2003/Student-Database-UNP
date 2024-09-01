#ifndef COURSE_H
#define COURSE_H

#include "student.h"

void add_course(StudentDB* db, int roll_no, int course_code, int marks);
void modify_course(StudentDB* db, int roll_no, int course_code, int new_marks);
void delete_course(StudentDB* db, int roll_no, int course_code);
void print_courses(StudentDB* db, int roll_no);


#endif