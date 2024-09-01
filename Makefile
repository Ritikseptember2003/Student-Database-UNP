CC = gcc -Wall
TARGET = 2101_2115
SRC = src/main.c src/parser.c src/student.c src/course.c
HEADERS = include/student.h include/course.h include/parser.h
OBJ = main.o parser.o student.o course.o

# Default target: compile the program
all: $(TARGET)

# Target to compile the program
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) -I include

# Compile each source file into an object file
main.o: src/main.c $(HEADERS)
	$(CC) -c src/main.c -I include

parser.o: src/parser.c $(HEADERS)
	$(CC) -c src/parser.c -I include

student.o: src/student.c $(HEADERS)
	$(CC) -c src/student.c -I include

course.o: src/course.c $(HEADERS)
	$(CC) -c src/course.c -I include

# Clean target to remove compiled files
clean:
	rm -f *.o $(TARGET) 2101_2115.out
