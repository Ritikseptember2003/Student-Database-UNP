#include "parser.h"
#include "client.h"
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

void intial_student(char* line) {
    if (sscanf(line, "%d, %99[^,], %f, %d", &roll_no, name, &cgpa, &no_of_course) == 4) {
        add_student(roll_no, name, cgpa, no_of_course);  
        int count = 0;
        while (count < no_of_course) {
            if (fgets(line, sizeof(line), file) == NULL) {
                printf("Error reading course data for student %d\n", roll_no);
                break;
            }
            if (sscanf(line, "%d, %d", &course_code, &marks) == 2) {
                add_course(roll_no, course_code, marks); 
            }else{
            	continue;
            }
            count++;
        }
    }
}


void parse_file(const char *filename) {
    file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
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
        } else if (strstr(line, "add student")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "#")) {
                    break;
                }
                if (sscanf(line, "%d, %99[^,], %f, %d", &roll_no, name, &cgpa, &no_of_course) == 4) {
                    add_student(roll_no, name, cgpa, no_of_course);  
                }
            }
        } else if (strstr(line, "add course")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "#")) {
                    break;
                }
                if (sscanf(line, "%d, %d, %d", &roll_no, &course_code, &marks) == 3) {
                    add_course(roll_no, course_code, marks);  
                }
            }
        } else if (strstr(line, "modify student")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "#")) {
                    break;
                }
                if (sscanf(line, "%d, %f", &roll_no, &new_cgpa) == 2) {
                    modify_student(roll_no, new_cgpa);  
                }
            }
        } else if (strstr(line, "modify course")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "#")) {
                    break;
                }
                if (sscanf(line, "%d, %d, %d", &roll_no, &course_code, &new_marks) == 3) {
                    modify_course(roll_no, course_code, new_marks); 
                }
            }
        } else if (strstr(line, "delete student")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "#")) {
                    break;
                }
                if (sscanf(line, "%d", &roll_no) == 1) {
                    delete_student(roll_no);  
                }
            }
        } else if (strstr(line, "delete course")) {
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "#")) {
                    break;
                }
                if (sscanf(line, "%d, %d", &roll_no, &course_code) == 2) {
                    delete_course(roll_no, course_code);  
                }
            }
        }
    }

    fclose(file);
}