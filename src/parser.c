#include "../include/student.h"
#include "../include/course.h"
#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF 2000

int roll_no, no_of_course;
char name[100];
float cgpa;
int course_code, new_marks, marks;  
float new_cgpa;


FILE *file;

void intial_student(char* line){
    
    if (sscanf(line, "%d, %99[^,], %f, %d", &roll_no, name, &cgpa, &no_of_course) == 4) {
        add_student(db, roll_no, name, cgpa, 0);
        int count = 0;
        while (count < no_of_course) {
            if (fgets(line, sizeof(line), file) == NULL) {
                printf("Error reading course data for student %d\n", roll_no);
                break;
            }
            if (sscanf(line, "%d, %d", &course_code, &marks) == 2) {
                add_course(db, roll_no, course_code, marks);
            } else {
                continue;
            }
            count++;
        }
    } 
}

void parse_file(char *filename1) {
    if(!db){
        db = create_student_db();
    }
    
    file = fopen(filename1, "r");
    if (!file) {
        printf("Error opening file %s\n", filename1);
        return;
    }

    char line[BUFF];
    while (!feof(file)) {
        if(!strstr(line,"#")){
           fgets(line, sizeof(line), file);
        }

        if (strstr(line, "initial")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "#")) {
                    break;
                }
                intial_student(line);
                
            }
        } if (strstr(line, "add student")) {
            while(fgets(line,sizeof(line),file)){
                if(strstr(line,"#")){
                    break;
                }
                if (sscanf(line, "%d, %99[^,], %f, %d", &roll_no, name, &cgpa, &no_of_course) == 4) {
                    add_student(db, roll_no, name, cgpa, 0);
                } 
            }
        } if (strstr(line, "add course")) {
            while(fgets(line,sizeof(line),file)){
                if(strstr(line,"#")){
                    break;
                }
                if (sscanf(line, "%d, %d, %d", &roll_no, &course_code, &marks) == 3) {
                    add_course(db, roll_no, course_code, marks);
                } 
            }
        } if (strstr(line, "modify student")) {
            while(fgets(line,sizeof(line),file)){
                if(strstr(line,"#")){
                    break;
                } if (sscanf(line, "%d, %f", &roll_no, &new_cgpa) == 2) {
                    modify_student(db, roll_no, new_cgpa);
                } 
            }
        } if (strstr(line, "modify course")) {
            while(fgets(line,sizeof(line),file)){
                if(strstr(line,"#")){
                    break;
                } if (sscanf(line, "%d, %d, %d", &roll_no, &course_code, &new_marks) == 3) {
                    modify_course(db, roll_no, course_code, new_marks);
                } 
            }
        } if (strstr(line, "delete student")) {
            while(fgets(line,sizeof(line),file)){
                if(strstr(line,"#")){
                    break;
                } if (sscanf(line, "%d", &roll_no) == 1) {
                    delete_student(db, roll_no);
                } 
            }
        } if (strstr(line, "delete course")) {
            while(fgets(line,sizeof(line),file)){
                if(strstr(line,"#")){
                    break;
                } if (sscanf(line, "%d, %d", &roll_no, &course_code) == 2) {
                    delete_course(db, roll_no, course_code);
                } 
            }
        }
    }

    fclose(file);
}

void write_output(){
    
    FILE *file_write = fopen("2101_2115.out", "w");
    if (!file_write) {
        printf("Error opening file \n");
        return;
    }

    Student *current = db->head;
    if (current == NULL) {
        printf("No students found in the database.\n");
        fclose(file_write);
        return;
    }

    while (current) {
        fprintf(file_write, "%d, %s, %.2f, %d\n", current->student_info.roll_no, current->student_info.name, current->student_info.cgpa, current->student_info.no_of_course);
        Course *course = current->courses;
        while (course) {
            fprintf(file_write, "%d, %d\n", course->course_code, course->marks);
            course = course->next;
        }
        current = current->next;
    }

    fclose(file_write);
}