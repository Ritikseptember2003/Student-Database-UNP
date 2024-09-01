#include "../include/student.h"
#include "../include/course.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StudentDB* db = NULL;

StudentDB* create_student_db() {
    db = (StudentDB*)malloc(sizeof(StudentDB));
    if (db == NULL) {
        printf("Memory allocation failed for student database.\n");
        return NULL;
    }
    db->head = NULL;
    db->tail = NULL;
    return db;
}

Student* find_student(StudentDB* db, int roll_no) {
    if (!db) {
        printf("Student database doesn't exist\n");
        return NULL;
    }

    Student *temp = db->head;
    while (temp != NULL) {
        if (temp->student_info.roll_no == roll_no) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void add_student(StudentDB *db, int roll_no, const char *name, float cgpa, int no_of_course) {
    if (!db) {
        printf("Student database doesn't exist\n");
        return;
    }

    if (find_student(db, roll_no)) {
        printf("Student already exists\n");
        return;
    }

    Student *new_student = (Student *)malloc(sizeof(Student));
    if (!new_student) {
        printf("Memory allocation failed for new student.\n");
        return;
    }

    new_student->student_info.roll_no = roll_no;
    snprintf(new_student->student_info.name, sizeof(new_student->student_info.name), "%s", name);
    new_student->student_info.cgpa = cgpa;
    new_student->student_info.no_of_course = no_of_course;
    new_student->courses = NULL;
    new_student->next = NULL;
    new_student->prev = NULL;

    if (db->head == NULL) {
        db->head = new_student;
        db->tail = new_student;
    } else {
        db->tail->next = new_student;
        new_student->prev = db->tail;
        db->tail = new_student;
    }
}

void modify_student(StudentDB* db, int roll_no, float new_cgpa) {
    if (!db) {
        printf("Student doesn't exist\n");
        return;
    }
    Student *student = find_student(db, roll_no);
    if (student) {
        student->student_info.cgpa = new_cgpa;
    }
}

void delete_student(StudentDB* db, int roll_no) {
    if (!db) {
        printf("Student database doesn't exist\n");
        return;
    }

    Student *student = find_student(db, roll_no);
    if (!student) {
        printf("Student doesn't exist\n");
        return;
    }

    if (student->prev) {
        student->prev->next = student->next;
    } else {
        db->head = student->next;
    }

    if (student->next) {
        student->next->prev = student->prev;
    } else {
        db->tail = student->prev;
    }

    Course *course = student->courses;
    while (course) {
        Course *next_course = course->next;
        free(course);
        course = next_course;
    }
    free(student);
}