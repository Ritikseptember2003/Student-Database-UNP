#include "client.h"
#include "message.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include <signal.h>
#include <arpa/inet.h>  
#include <errno.h>      


int sockfd;  
Message msg;

void handle_sigpipe(int sig) {
    printf("Server either not running or closed the connection\n");
    exit(EXIT_FAILURE);
}

void init_socket(const char *server_ip, int server_port) {
    struct sockaddr_in server_addr;

    // Setup signal handler for SIGPIPE
    if (signal(SIGPIPE, handle_sigpipe) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    // Create a TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);  
        exit(EXIT_FAILURE);
    }
}

void close_socket() {
    if (close(sockfd) == -1) {
        perror("close socket");
    }
}

void send_message(Message *msg) {
    //printf("Sending message of type: %d\n", msg->type);
    if (send(sockfd, msg, sizeof(Message), 0) == -1) {
        perror("send to socket");
        close_socket();  
        exit(EXIT_FAILURE);
    }
}

void add_student(int roll_no, const char *name, float cgpa, int no_of_courses) {
    msg.type = ADD_STUDENT;
    msg.data.addstu.roll_no = roll_no;
    strncpy(msg.data.addstu.name, name, sizeof(msg.data.addstu.name) - 1);
    msg.data.addstu.name[sizeof(msg.data.addstu.name) - 1] = '\0'; 
    msg.data.addstu.cgpa = cgpa;
    msg.data.addstu.no_of_courses = no_of_courses;
    send_message(&msg);
}

void add_course(int roll_no, int course_code, int marks) {
    msg.type = ADD_COURSE;
    msg.data.addco.roll_no = roll_no;
    msg.data.addco.course_code = course_code;
    msg.data.addco.marks = marks;
    send_message(&msg);
}

void modify_student(int roll_no, float new_cgpa) {
    msg.type = MODIFY_STUDENT;
    msg.data.modstu.roll_no = roll_no;
    msg.data.modstu.new_cgpa = new_cgpa;
    send_message(&msg);
}

void modify_course(int roll_no, int course_code, int new_marks) {
    msg.type = MODIFY_COURSE;
    msg.data.modco.roll_no = roll_no;
    msg.data.modco.course_code = course_code;
    msg.data.modco.new_marks = new_marks;
    send_message(&msg);
}

void delete_student(int roll_no) {
    msg.type = DELETE_STUDENT;
    msg.data.delstu.roll_no = roll_no;
    send_message(&msg);
}

void delete_course(int roll_no, int course_code) {
    msg.type = DELETE_COURSE;
    msg.data.delco.roll_no = roll_no;
    msg.data.delco.course_code = course_code;
    send_message(&msg);
}

int main(int argc,char *argv[]){

    if(argc<4)
    {
        perror("usage:<client IP> <client port> <input file>\n");
        exit(EXIT_FAILURE);
    }

    init_socket(argv[1],atoi(argv[2]));

    
    parse_file(argv[3]);
    
    exit(0);

}