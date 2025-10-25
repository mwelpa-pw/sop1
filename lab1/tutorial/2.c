#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 1000

int scandirf() {
    struct dirent * dp;
    int files = 0, dirs = 0, links = 0, other = 0; 

    DIR *dirp = opendir(".");

    if (dirp == NULL)
        return(EXIT_FAILURE);
    while ((dp = readdir(dirp)) != NULL) { // outputs files till the end (NULL)
        switch (dp->d_type) {
            case 4:
                dirs++;
                break;
            case 10:
                links++;
                break;
            case 8:
                files++;
                break;
            default:
                other++;
        }
        printf("name: %s\n", dp->d_name);
    }
    if (closedir(dirp) == -1) {
        return(EXIT_FAILURE);
    }
    printf("Dirs: %d, Links: %d, Files: %d, Other files: %d\n", dirs, links, files, other);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        printf("Results for: %s\n", argv[i]);
        if (chdir(argv[i])) {
            return EXIT_FAILURE;
        }
        if (scandirf()) {
            return(EXIT_FAILURE);
        }
    }

    return(EXIT_SUCCESS);
}
