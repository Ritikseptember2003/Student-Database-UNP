#include "../include/parser.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    parse_file(argv[1]);
    write_output();

    return 0;
}