#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define ERR(source) \
    (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))
// pid_t fork(void). pid_t - int, u rodzica pid, w utworzonym procesie 0, fail -1
// wait(null) czeka na pierwsze dziecko, waitpid(null,pid, options) czeka na dziecko z pidem, pid = -1 dowolny potomny proces, pid == 0 dowolny nalezacy
// albo errno -> ECHILD = nie ma dzieci juz
// do gripy procesu wywolujacego pid < -1 czekamy na dowolny proces ktorego identyfikator grupu jest rowny wartosci bezwzglednej pid. 
// options:WCONTINUED - Funkcja powinna zwrocic inf o procesach, ktore zostaly wznowione po zatrzymaniu
//         WNOHANG - waitpid nie powinna zatrzymac wywolania biezacego procesu (zwroci nam 0)
//         WUNTRACED - funkcja ma zwrocic info o procesach, ktore zostaly zatrzymanejjjjlkk

// z child wziac pid rodzic getppid(), getpid() swoj pid

// co zwraca waitpid: >0 - pid dziecka ktore sie zakonczylo, 0 - WHOHANG uzyte i nic sie nie stalo, -1 - BŁąD

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Too few arguments. Exiting...");
        ERR("Too few arguments");
    }
    int n = atoi(argv[1]);
    int pid;

    for (int i = 0; i < n; i++) { 
        if ((pid = fork()) == 0) { // child
            srand(time(NULL) ^ getpid()); // wpp wszystkie dzieci maja ten sam seed
            int r = rand() % 5 + 5;
            sleep(r);
            printf("I am child, my PID is %d. I was %d seconds alive.\n ", getpid(), r);
            exit(EXIT_SUCCESS);
        } else if (pid == -1)
            ERR("Fork: ");
        else { // parent
            waitpid(pid, NULL, 0);
            printf("I am parent, my PID is %d. My child %d just finished its' life.\n", getpid(), pid);
        }
    }
    return EXIT_SUCCESS;
}