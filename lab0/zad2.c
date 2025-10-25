#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXL 20
#define ERR(source) (perror(source), \
                    fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), \
                    exit(EXIT_FAILURE))

void usage(char *pname) {
    fprintf(stderr, "USAGE:%s name times>-\n", pname);
    exit(EXIT_FAILURE); // dlaczego tutaj exit, a nie return?
}

int main(int argc, char **argv) {
    if (argc != 3)
        usage(argv[0]);
    int i, j = atoi(argv[2]);
    if (j <= 0)
        usage(argv[0]);
    for (i = 0; i < j; i++)
        printf("Hello %s\n", argv[1]);
    return EXIT_SUCCESS;
}
