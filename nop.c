
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char FILENAME[] = "test";

int main(void) {
    size_t bytes;
    FILE *fd;
    unsigned char *buf;

    if ((fd = fopen(FILENAME, "rb")) == NULL) {
        fprintf(stderr, "error opening file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    fseek(fd, 0, SEEK_END);
    bytes = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    if ((buf = malloc(bytes)) == NULL) {
        fprintf(stderr, "memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    if (fread(buf, 1, bytes, fd) != bytes) {
        fprintf(stderr, "error reading file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    if (fclose(fd) == EOF) {
        fprintf(stderr, "error closing file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    memset(buf + 0x1000, 0x90, 0x2e);

    if ((fd = fopen(FILENAME, "wb")) == NULL) {
        fprintf(stderr, "error opening file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    if (fwrite(buf, 1, bytes, fd) != bytes) {
        fprintf(stderr, "error writing file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    if (fclose(fd) == EOF) {
        fprintf(stderr, "error closing file [%s]\n", FILENAME);
        exit(EXIT_FAILURE);
    }

    free(buf);

    return EXIT_SUCCESS;
}

