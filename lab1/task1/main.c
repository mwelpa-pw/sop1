#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

char* create_path(char *before, char *after) {
    char *fullpath = (char*)malloc(strlen(before) + strlen(after) + 2); // +1 bo /0; +1 bo "/" ===> +2
    if (fullpath == NULL)
        ERR("fullpath err");
    sprintf(fullpath, "%s/%s", before, after);
    return fullpath;
}

void list_files_for(char *path, FILE *out_place) {
    DIR *dirp;
    struct dirent *dp;
    struct stat file_stat_buf;
    if ((dirp = opendir(path)) == NULL)
        ERR("opendir");
    
    fprintf(out_place, "SCIEZKA:\n%s\nLISTA PLIKÓW:\n", path);
    while ((dp = readdir(dirp)) != NULL) {
        char* full_path = create_path(path, dp->d_name);
        if (stat(full_path, &file_stat_buf) != 0)
            ERR("stat");

        fprintf(out_place, "%s %lld\n", dp->d_name, file_stat_buf.st_size);
        free(full_path);
    }


    if (closedir(dirp))
        ERR("closedir");
}

int main(int argc, char** argv) {
    int c;
    char *default_dir = "./";
    FILE *filename = stdout; 
    int found_p = 0, found_o = 0;

    while ((c = getopt(argc, argv, "o:p:")) != -1)
        switch(c) {
            case 'o':
                if (found_o)
                    continue;
                found_o++;
                filename = fopen(optarg, "a");
                if (filename == NULL)
                    ERR("fopen -o");
                break;
            case 'p':
                found_p++;
                list_files_for(optarg, filename);
                break;
            case '?':
                ERR("Błąd: Nieznana opcja lub brak argumentu.");
            default:
                ERR("arg not known");
        }

    if (!found_p) {
        list_files_for(default_dir, filename);
    }

    if (found_o)
        fclose(filename);

} 