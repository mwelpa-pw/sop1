#include <dirent.h>
#include <errno.h>
#include <ftw.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define _XOPEN_SOURCE 500 // potrzebne dla linuxa !!!

char* dir = ".";

int dirs = 0, files = 0, links = 0, other = 0;

int walk(const char *name, const struct stat *s, int type, struct FTW *f) {
    switch (type) {
        case FTW_D:
            dirs++;
            break;
        case FTW_F:
            files++;
            break;
        case FTW_SL:
            links++;
            break;
        default:
            other++;
    }

    return 0;
}

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (nftw(argv[i], walk, 500, FTW_PHYS) == 0) // nftw samo wchodzi w kolejne katalogi i odpala samo z siebie ntfw, MAX_FD stala max deskryptorow
        // FTW_PHYS flaga ktora mowi zeby nie sledzic dowiazac symbolicznych (man nftw), walk <- wskaznik na funkcje ktora bedzie wolana dla kazdego el
        // argv[i] sciezka poczatkowa
            printf("%s:\nfiles:%d\ndirs:%d\nlinks:%d\nother:%d\n", argv[i], files, dirs, links, other);
        else
            printf("%s: access denied\n", argv[i]);
        dirs = files = links = other = 0;
    }
    return EXIT_SUCCESS;
}