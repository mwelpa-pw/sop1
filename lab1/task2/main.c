#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <string.h>
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

FILE *place;
int ext_ = 0, depth = 1;
char *ext;

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

int callback_fn(const char* callback_path, const struct stat *stat_ptr, int type_of_file, struct FTW *level) {
    if (type_of_file == FTW_D) 
        fprintf(place, "\npath: %s\n", callback_path);
    else if (type_of_file == FTW_F) {
        if (ext_) {
            const char* f_ext = get_filename_ext(callback_path);
            if (!strcmp(f_ext, ext)) 
                fprintf(place, "%s %lld\n", callback_path, stat_ptr->st_size);
        } else {
            fprintf(place, "%s %lld\n", callback_path, stat_ptr->st_size);
        }
    } else {
        ERR("File not known");
    }

    return 0;
}

void search_dirs(char *path) {
    fprintf(place, "path: %s\n", path);
    nftw(path, callback_fn, depth, FTW_PHYS);
}

// int add_to_buff

int main(int argc, char **argv) {

    place = stdout;
    int file_opened = 0, i = 0, c;
    char *dirs[1000];
    while ((c = getopt(argc, argv, "p:e:d:o")) != -1)
        switch (c) {
            case 'p':
                // search_dirs(optarg);
                dirs[i++] = optarg;
                break;
            case 'e':
                ext_++;
                ext = optarg;
                break;
            case 'd':
                depth = atoi(optarg);
                break;
            case 'o':
                file_opened++;
                place = fopen(getenv("L1_OUTPUFILE"), "w");
                break;
            case '?':
                ERR("wrong params");
        }

    for (; i > 0; i--) {
        search_dirs(dirs[i-1]);
    }
    
    if (file_opened)
        fclose(place);
}