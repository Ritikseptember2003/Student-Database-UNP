#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h> 
#include "message.h" 
#include "student.h"
#include "course.h"

#define BACKLOG 5  

pthread_mutex_t db_mutex = PTHREAD_MUTEX_INITIALIZER;  

void write_output(StudentDB* db, const char* filename) {
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    if (db == NULL || db->head == NULL) {
        printf("Database is empty, nothing to write.\n");
        close(fd);
        return;
    }

    Student *current = db->head;
    while (current != NULL) {
        dprintf(fd, "%d, %s, %.2f, %d\n",
                current->student_info.roll_no,
                current->student_info.name,
                current->student_info.cgpa,
                current->student_info.no_of_course);

        Course *course = current->courses;
        while (course != NULL) {
            dprintf(fd, "%d, %d\n", course->course_code, course->marks);
            course = course->next;
        }
        current = current->next;
    }
    close(fd);
}

void load_data_from_file(StudentDB* db, const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return;
    }

    char student_line[256];
    while (fgets(student_line, sizeof(student_line), file)) {
        int roll_no;
        char name[100];
        float cgpa;
        int no_of_courses;

        if (sscanf(student_line, "%d, %99[^,], %f, %d", &roll_no, name, &cgpa, &no_of_courses) != 4) {
            fprintf(stderr, "Invalid student data format: %s", student_line);
            continue;
        }

        add_student(db, roll_no, name, cgpa, 0);

        for (int i = 0; i < no_of_courses; i++) {
            if (fgets(student_line, sizeof(student_line), file) == NULL) {
                fprintf(stderr, "Error reading course data for student %d\n", roll_no);
                break;
            }

            int course_code, marks;
            if (sscanf(student_line, "%d, %d", &course_code, &marks) != 2) {
                fprintf(stderr, "Invalid course data format: %s", student_line);
                continue;
            }

            add_course(db, roll_no, course_code, marks);
        }
    }
    fclose(file);
}

void handle_client(int client_socket, StudentDB* db) {
    Message msg;
    while (1) {
        ssize_t bytesRead = recv(client_socket, &msg, sizeof(Message), 0);
        if (bytesRead > 0) {
            pthread_mutex_lock(&db_mutex);  
            switch (msg.type) {
                case ADD_STUDENT:
                    add_student(db, msg.data.addstu.roll_no, msg.data.addstu.name, msg.data.addstu.cgpa, 0);
                    break;
                case ADD_COURSE:
                    add_course(db, msg.data.addco.roll_no, msg.data.addco.course_code, msg.data.addco.marks);
                    break;
                case MODIFY_STUDENT:
                    modify_student(db, msg.data.modstu.roll_no, msg.data.modstu.new_cgpa);
                    break;
                case MODIFY_COURSE:
                    modify_course(db, msg.data.modco.roll_no, msg.data.modco.course_code, msg.data.modco.new_marks);
                    break;
                case DELETE_STUDENT:
                    delete_student(db, msg.data.delstu.roll_no);
                    break;
                case DELETE_COURSE:
                    delete_course(db, msg.data.delco.roll_no, msg.data.delco.course_code);
                    break;
                default:
                    fprintf(stderr, "Unknown message type %d\n", msg.type);
                    break;
            }
            pthread_mutex_unlock(&db_mutex);  
        } else if (bytesRead == 0) {
            printf("Client disconnected.\n");
            break;
        } else {
            perror("recv");
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <server IP> <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[2]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, argv[1], &server_addr.sin_addr);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, BACKLOG) == -1) {
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        printf("Connected to client: %s\n", inet_ntoa(client_addr.sin_addr));

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            close(client_socket);
            continue;
        }

        if (pid == 0) { 
            close(server_socket);
            StudentDB db;
            create_student_db(&db);

            load_data_from_file(&db, "2101_2115.out");
            handle_client(client_socket, &db);
            write_output(&db, "2101_2115.out");

            close(client_socket);
            exit(EXIT_SUCCESS);
        }

        close(client_socket); 
    }

    close(server_socket);
    return 0;
}