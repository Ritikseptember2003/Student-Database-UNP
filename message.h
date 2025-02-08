#ifndef MESSAGE_H
#define MESSAGE_H


typedef enum {
    ADD_STUDENT,
    MODIFY_STUDENT,
    DELETE_STUDENT,
    ADD_COURSE,
    MODIFY_COURSE,
    DELETE_COURSE,
    QUIT
} MessageType;


typedef struct {
    MessageType type;  
    int roll_no;
    char name[100];
    float cgpa;
    int no_of_courses;
} AddStudentData;

typedef struct {
    MessageType type;
    int roll_no;
    float new_cgpa;
} ModifyStudentData;

typedef struct {
    MessageType type;
    int roll_no;
} DeleteStudentData;

typedef struct {
    MessageType type;
    int roll_no;
    int course_code;
    int marks;
} AddCourseData;

typedef struct {
    MessageType type;
    int roll_no;
    int course_code;
    int new_marks;
} ModifyCourseData;

typedef struct {
    MessageType type;
    int roll_no;
    int course_code;
} DeleteCourseData;



typedef struct {
    MessageType type;
    union {
       	AddStudentData addstu;
       	ModifyStudentData modstu;
       	DeleteStudentData delstu;
       	AddCourseData addco;
       	ModifyCourseData modco;
       	DeleteCourseData delco;      
    } data;
} Message;


#endif