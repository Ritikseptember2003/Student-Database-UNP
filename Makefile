CC = gcc -Wall

SERVER_TARGET = server
PARSER_TARGET = parser

SERVER_SRC = server.c student.c course.c
PARSER_SRC = parser.c client.c

SERVER_OBJ = $(SERVER_SRC:.c=.o)
PARSER_OBJ = $(PARSER_SRC:.c=.o)

OUTPUT_FILE = 2101_2115.out

all: $(SERVER_TARGET) $(PARSER_TARGET)

$(SERVER_TARGET): $(SERVER_OBJ)
	$(CC) -o $(SERVER_TARGET) $(SERVER_OBJ)

$(PARSER_TARGET): $(PARSER_OBJ)
	$(CC) -o $(PARSER_TARGET) $(PARSER_OBJ)

%.o: %.c
	$(CC) -c $<

run-server: $(SERVER_TARGET)
	./$(SERVER_TARGET)

run-client: $(PARSER_TARGET)
	./$(PARSER_TARGET) input.txt > $(OUTPUT_FILE)

clean:
	rm -f *.o $(SERVER_TARGET) $(PARSER_TARGET) $(OUTPUT_FILE)