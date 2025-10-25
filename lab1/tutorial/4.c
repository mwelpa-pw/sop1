#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define RANDMAX 60


int main(int argc, char **argv) {
    int i = 1;
    char *name = NULL;
    int permission = 0644;
    int size = 10;
    srand(time(NULL));

    while (i < argc) {
        if (i + 1 >= argc)
            return EXIT_FAILURE;

        if (i % 2) { // getopt tu mozna bylo uzyc
            if (!strcmp(argv[i],"-n")) 
                name = argv[i+1];
            else if (!strcmp(argv[i],"-p")) 
                permission = strtol(argv[i+1], NULL, 8);
            else if (!strcmp(argv[i],"-s")) {
                size = atoi(argv[i+1]);
                if (size < 1)
                    return EXIT_FAILURE;
            }
            else
                return EXIT_FAILURE;
        }
        i += 2;
    }

    if (name == NULL) {
        fprintf(stderr, "Nie podano nazwy pliku (-n)\n");
        return EXIT_FAILURE;
    }

    if (unlink(name)) // zeby usunac plik z innymi upraiwnieniami <- tak naprawde to sciema i PRETEKST
        return EXIT_FAILURE;

    FILE *filew = fopen(name, "w");

    if (filew == NULL)
        return EXIT_FAILURE;
    if (fchmod(fileno(filew), permission) != 0)
        return EXIT_FAILURE;

    for (int j = 0; j < size; j++) {
        char c = '\0';
        if ((rand() % 100) < 10) {
            c = rand() % 52 + 'A';
        }

        if (fputc(c,filew) == EOF)
            return EXIT_FAILURE;
    }

    printf("Seed = %ld\n", time(NULL));

    if (fputc('\0',filew) == EOF)
            return EXIT_FAILURE;

    if (fclose(filew) == EOF)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}