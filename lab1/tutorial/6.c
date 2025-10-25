#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> 

#define MAXLINE 1000

int main (int argc, char **argv) {
    char *path1 = argv[1];
    char *path2 = argv[2];

    char data[MAXLINE];

    int fd_first = open(path1, O_RDONLY); //offset ustawiany na 0
    int fd_second = open(path2, O_WRONLY);

    if (fd_first == -1 || fd_second == -1) 
        return 1;

    ssize_t k;
    while ((k = read(fd_first, data, MAXLINE)) > 0) { // offset w fd_first jest przesuwany o k do przodu
        if (write(fd_second, data, k) != k) { // offset w fd_second jest przesuwany po zapisie k bajtow o k do przodu
            fprintf(stderr, "Blad zapisu");
            return 1;
        } 
    }

    close(fd_first);
    close(fd_second);

    return 0;
}