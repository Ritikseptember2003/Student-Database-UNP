#ifndef STUDENT_H
#define STUDENT_H

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
} StudentDB;

extern StudentDB* db;

StudentDB* create_student_db();
void destroy_student_db(StudentDB* db);
void add_student(StudentDB* db, int roll_no, const char *name, float cgpa, int no_of_course);
void modify_student(StudentDB* db, int roll_no, float new_cgpa);
void delete_student(StudentDB* db, int roll_no);
Student* find_student(StudentDB* db, int roll_no);
void print_students(StudentDB *db);

#endif