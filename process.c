
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

#define BYTES 0x2e

const char BLOB[] = "blob";
const char TEST[] = "test";

int main(void) {
    pid_t pid;
    size_t size;
    int status;
    unsigned char buf[BYTES];
    unsigned char *prog;
    FILE *fd;
    struct stat statbuf;
    struct utimbuf timebuf;

    if (stat(TEST, &statbuf) == -1) {
        fprintf(stderr, "error getting file stats [%s]\n", TEST);
        exit(EXIT_FAILURE);
    }

    timebuf.actime = statbuf.st_mtime;
    timebuf.modtime = statbuf.st_mtime;

    if ((fd = fopen(TEST, "rb")) == NULL) {
        fprintf(stderr, "error opening file [%s]\n", TEST);
        exit(EXIT_FAILURE);
    }

    fseek(fd, 0, SEEK_END);
    size = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    if ((prog = malloc(size)) == NULL) {
        fprintf(stderr, "memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    if (fread(prog, 1, size, fd) != size) {
        fprintf(stderr, "error reading file [%s]\n", TEST);
        exit(EXIT_FAILURE);
    }

    if (fclose(fd) == EOF) {
        fprintf(stderr, "error closing file [%s]\n", TEST);
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) == -1) {
        fprintf(stderr, "process creation failed\n");
        exit(EXIT_FAILURE);
    }

    if (!pid) {
        if ((fd = fopen(BLOB, "rb")) == NULL) {
            fprintf(stderr, "error opening file [%s]\n", BLOB);
            exit(EXIT_FAILURE);
        }

        if (fread(buf, 1, BYTES, fd) != BYTES) {
            fprintf(stderr, "error reading file [%s]\n", BLOB);
            exit(EXIT_FAILURE);
        }

        if (fclose(fd) == EOF) {
            fprintf(stderr, "error closing file [%s]\n", BLOB);
            exit(EXIT_FAILURE);
        }

        memcpy(prog + 0x1000, buf, BYTES);

        if ((fd = fopen(TEST, "wb")) == NULL) {
            fprintf(stderr, "error opening file [%s]\n", TEST);
            exit(EXIT_FAILURE);
        }

        if (fwrite(prog, 1, size, fd) != size) {
            fprintf(stderr, "error writing file [%s]\n", TEST);
            exit(EXIT_FAILURE);
        }

        if (fclose(fd) == EOF) {
            fprintf(stderr, "error closing file [%s]\n", TEST);
            exit(EXIT_FAILURE);
        }

        free(prog);

        execl(TEST, TEST, (char *)0);
    } 
    else {
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));

        if ((fd = fopen(TEST, "wb")) == NULL) {
            fprintf(stderr, "error opening file [%s]\n", TEST);
            exit(EXIT_FAILURE);
        }

        if (fwrite(prog, 1, size, fd) != size) {
            fprintf(stderr, "error writing file [%s]\n", TEST);
            exit(EXIT_FAILURE);
        }

        if (fclose(fd) == EOF) {
            fprintf(stderr, "error closing file [%s]\n", TEST);
            exit(EXIT_FAILURE);
        }

        if (utime(TEST, &timebuf) == -1) {
            fprintf(stderr, "error setting modification time\n");
            exit(EXIT_FAILURE);
        }

        free(prog);
    }

    return EXIT_SUCCESS;
}

