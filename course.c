#include "../include/course.h"
#include "../include/student.h"
#include <stdio.h>
#include <stdlib.h>

Course* find_course(Student* student, int course_code) {
    if (!student) {
        printf("Course does not exist\n");
        return NULL;
    }

    Course *course = student->courses;
    while (course != NULL) {
        if (course->course_code == course_code) {
            return course;
        }
        course = course->next;
    }
    return NULL;
}

void add_course(StudentDB* db, int roll_no, int course_code, int marks) {
    Student *student = find_student(db, roll_no);
    if (!student) {
        printf("Student doesn't exist\n");
        return;
    }

    if (find_course(student, course_code)) {
        printf("Duplicate course code %d for student %d.\n", course_code, roll_no);
        return;
    }

    Course *new_course = (Course*)malloc(sizeof(Course));
    if (!new_course) {
        printf("Memory allocation failed for new course.\n");
        return;
    }

    new_course->course_code = course_code;
    new_course->marks = marks;
    new_course->next = NULL;
    new_course->prev = NULL;

    if (student->courses == NULL) {
        student->courses = new_course;
    } else {
        Course *temp = student->courses;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_course;
        new_course->prev = temp;
    }
    student->student_info.no_of_course++;
}

void modify_course(StudentDB* db, int roll_no, int course_code, int new_marks) {
    Student *student = find_student(db, roll_no);
    if (!student) return;

    Course *course = find_course(student, course_code);
    if (course) {
        course->marks = new_marks;
    }
}

void delete_course(StudentDB* db, int roll_no, int course_code) {
    Student *student = find_student(db, roll_no);
    if (!student) return;

    Course *course = find_course(student, course_code);
    if (!course) return;

    if (course->prev) {
        course->prev->next = course->next;
    } else {
        student->courses = course->next;
    }
    if (course->next) {
        course->next->prev = course->prev;
    }
    free(course);
    student->student_info.no_of_course--;
}