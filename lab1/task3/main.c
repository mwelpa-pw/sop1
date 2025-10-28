#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ftw.h>
#include <string.h>
#include <sys/stat.h>
#define ERR(source) (perror(source), fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), exit(EXIT_FAILURE))

char *env_var;

char* create_path(const char *before, const char *after) {
    char *fullpath = (char*)malloc(strlen(before) + strlen(after) + 2); // +1 bo /0; +1 bo "/" ===> +2
    if (fullpath == NULL)
        ERR("fullpath err");
    sprintf(fullpath, "%s/%s", before, after);
    return fullpath;
}

const char *getPkgName(const char* line) {
    char* line_copy = (char*)malloc(strlen(line) + 1);
    strcpy(line_copy, line);
    char* temp = strchr(line_copy, '=');
    if (!temp)
        return line_copy;
    temp[0] = '\0';
    return line_copy;
}

void createNewEnv(char *path) {
    mkdir(path, 07777);
    char *fullpath = create_path(path, "requirements");
    FILE *opened_file = fopen(fullpath, "w");
    if (opened_file == NULL)
        ERR("fopen err");
    fprintf(opened_file, "\n\n");
    fclose(opened_file);
}

void createPkgFile(const char *name) { // tworzenie pliku
    char *fullpath = create_path(env_var, name);
    mode_t old_mask = umask(0333);
    
    FILE *opened_file = fopen(fullpath, "a");
    if (opened_file == NULL)
        ERR("fopen err");

    char buf[1000];
    for (int i = 0; i < 1000; i++)
        buf[i] = rand() % 52 + 'A';
    fprintf(opened_file, "%s\n", buf);

    umask(old_mask);
    fclose(opened_file);
}

int checkExistence(const char* pkgName) {
    if (access(create_path(env_var, pkgName), F_OK))
        return 1;
    return 0;
}

void installPkg(char *line) { // dopisywanie srodowiska
    if (!checkExistence(getPkgName(line)))
        ERR("Pkg already exists!");

    char *fullpath = create_path(env_var, "requirements");
    FILE *opened_file = fopen(fullpath, "a");
    if (opened_file == NULL)
        ERR("fopen err");

    createPkgFile(getPkgName(line));

    printf("%s", line);

    fprintf(opened_file, "%s", line);
    fclose(opened_file);
}

void rmPkg(char *line) {
    char buf[1001];
    char *fullpath = create_path(env_var, "requirements");
    FILE* opened_file = fopen(fullpath, "r");
    size_t bytes_read = fread(buf, sizeof(char), 1000, opened_file);
    fclose(opened_file);

    buf[bytes_read] = '\0';
    int prev_n = 0, post_n = -1;

    for (int i = 0; buf[i] != '\0' && i < 1001; i++) {
        if (buf[i] == '\n' || buf[i] == '\0') {
            prev_n = post_n + 1;
            post_n = i;

            int length = post_n - prev_n;

            char* temp_name = (char*)malloc(sizeof(char) * (length+1));
            for (int j = 0; j < length; j++) {
                temp_name[j] = buf[prev_n + j];
            }
            temp_name[length] = '\0';

            printf("%s", temp_name);

            if (strcmp(getPkgName(temp_name), getPkgName(line)) == 0) {
                memmove(buf + prev_n, buf + post_n + 1, strlen(buf + post_n + 1) + 1);
            }
        }
    }

    opened_file = fopen(fullpath, "w");
    bytes_read = fprintf(opened_file, "%s", buf);
    fclose(opened_file);

    fullpath = create_path(env_var, getPkgName(line));
    unlink(fullpath);
    rmdir(env_var);
}

int main(int argc, char **argv) {
    int c;

    char *all_envs[1000]; 
    int all_envsIt = 0;

    char *PkgsToInstall[1000];
    int PkgsToInstallIt = 0;

    char *PkgsToRm[1000];
    int PkgsToRmIt = 0;

    while ((c = getopt(argc, argv, "c:v:i:r:")) != -1)
        switch (c) {
            case 'c':
                createNewEnv(optarg);
                break;
            case 'v':
                all_envs[all_envsIt++] = optarg;
                break;
            case 'i':
                PkgsToInstall[PkgsToInstallIt++] = optarg;
                break;
            case 'r':
                PkgsToRm[PkgsToRmIt++] = optarg;
                break;
        }

    for (int j = 0; j < all_envsIt; j++) {
        env_var = all_envs[j];
        for (int k = 0; k < PkgsToInstallIt; k++) {
            installPkg(PkgsToInstall[k]);
        }
        for (int k = 0; k < PkgsToRmIt; k++) {
            rmPkg(PkgsToRm[k]);
        }
    }
}
