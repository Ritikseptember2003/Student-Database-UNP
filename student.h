#ifndef STUDENT_H
#define STUDENT_H

#include<pthread.h>
#define MAXSTU 100

typedef struct Course {
    int course_code;
    int marks;
    struct Course *next;
    struct Course *prev;
} Course;

typedef struct StudentInfo{
    int roll_no;
    char name[100];
    float cgpa;
    int no_of_course;
}StudentInfo;

typedef struct Student {
    StudentInfo student_info;
    Course *courses;
    struct Student *next;
    struct Student *prev;
} Student;

typedef struct {
    Student *head;
    Student *tail;
    int student_count;
    pthread_mutex_t mutex;
    Student students[MAXSTU];
} StudentDB;

extern StudentDB* db;

StudentDB* create_student_db();
void add_student(StudentDB* db, int roll_no, const char *name, float cgpa, int no_of_course);
void modify_student(StudentDB* db, int roll_no, float new_cgpa);
void delete_student(StudentDB* db, int roll_no);
Student* find_student(StudentDB* db, int roll_no);
void print_students(StudentDB *db);

#endif 
