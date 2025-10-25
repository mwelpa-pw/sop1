#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXL 20
#define ERR(source) (perror(source), \
                    fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), \
                    exit(EXIT_FAILURE))

int main(int argc, char **argv) {
    int i;
    for (i = 1; i < argc; i++) //
        printf("%s\n", argv[i]);
    EXIT_SUCCESS;
}
