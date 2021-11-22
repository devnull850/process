
#include <stdio.h>
#include <stdlib.h>

#define BYTES 0x2e

const char INPUT[] = "test"; 
const char OUTPUT[] = "blob";

int main(void) {
    unsigned char buf[BYTES];
    FILE *fd;

    if ((fd = fopen(INPUT, "rb")) == NULL) {
        fprintf(stderr, "error opening file [%s]\n", INPUT);
        exit(EXIT_FAILURE);
    }

    fseek(fd, 0x1000, SEEK_SET);

    if (fread(buf, 1, BYTES, fd) != BYTES) {
        fprintf(stderr, "error reading file [%s]\n", INPUT);
        exit(EXIT_FAILURE);
    }

    if (fclose(fd) == EOF) {
        fprintf(stderr, "error closing file [%s]\n", INPUT);
        exit(EXIT_FAILURE);
    }

    if ((fd = fopen(OUTPUT, "wb")) == NULL) {
        fprintf(stderr, "error opening file [%s]\n", OUTPUT);
        exit(EXIT_FAILURE);
    }

    if (fwrite(buf, 1, BYTES, fd) != BYTES) {
        fprintf(stderr, "error writing file [%s]\n", OUTPUT);
        exit(EXIT_FAILURE);
    }

    if (fclose(fd) == EOF) {
        fprintf(stderr, "error closing file [%s]\n", OUTPUT);
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

