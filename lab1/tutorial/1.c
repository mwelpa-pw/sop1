#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    DIR *dirp = opendir(".");
    struct dirent * dp;
    int files = 0, dirs = 0, links = 0, other = 0; 

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
    return(EXIT_SUCCESS);
}
