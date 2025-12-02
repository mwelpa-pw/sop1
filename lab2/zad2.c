#include <string.h> // dla memset
#include <stdlib.h> // dla exit, atoi, rand
#include <stdio.h>  // dla printf
#include <unistd.h> // dla fork, sleep
#include <time.h>   // dla nanosleep
#include <signal.h> // dla sigaction
#include <sys/wait.h> // dla waitpid
#include <errno.h>  // dla errno
#define ERR(source) \
    (fprintf(stderr, "%s:%d\n", __FILE__, __LINE__), perror(source), kill(0, SIGKILL), exit(EXIT_FAILURE))

// int kill(pid_t pid, int sig); pid - proces do ktorego ma isc sygna (0 - wszystkie procesry 
// nalezace do grupy porcesow nadawcy i nadawca, -1 do wszystkich procesow
// do ktorych nadawca ma uprawanienia, < - 1 sygnal wyslany do prcesow o id grupy rownym modul 
// |-x|), sig - rodzaj sygnaly z makr np.
// 30    SIGUSR1          terminate process    User defined signal 1
// OUT: 0 gdy poprawne wykonanie, -1 gdy niepoprawne. errno jest ustawiane na costam

// C nie pozwala na tworzenie funkcji wewnatrz funkcji
// void Tutorial() {
//     struct sigaction k; // do ustawienia handlera
    
//     k.sa_handler = info; // info jest funkcja do handlowania

//     int p = sigaction(SIGUSR1, &k, NULL); // ustawia, ze USR1 jest obslugiwane przez k
//     // dla rodzica k, dla inne k

//     sigset_t l; // maska
//     // block - sumowanie masek
//     // unblock roznica
//     // setmask nadpisuje
//     sigemptyset(&l);
//     sigaddset(&l, SIGUSR1);
//     sigset_t oldmask;
//     sigprocmask(SIG_SETMASK, &l, &oldmask);
// }

/////////// ZADANIE
volatile sig_atomic_t lastsig;

void sethandler(void (*f)(int), int sigNo) {
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = f;

    if (-1 == sigaction(sigNo, &act, NULL))
        ERR("sigaction");
}


void info(int sig /*SIGUSR1*/) { lastsig = sig; } // SIG_IGN Signal Ignore 

// maska - sygnaly ktore sa blokowane

// signal() - obsluga sygnalu
int main(int argc, char **argv) { // n k p r
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    int p = atoi(argv[3]);
    int r = atoi(argv[4]);
    // 1. tworzy n procesow potomnych d
    // 2. rodzic wysyla do nich SIGUSR1 i SIGUSR2 po odpowiednio k i p sekundach d
    // 3.Konczy sie gdy koncza sie procesy potomne d
    // 4.Kazdy proces potomny losuje czas swojego sleep(5-10s) i wypisuje SUKCES jesli ostatnim otrzymanym 
    //      sygnalem byl SIGUSR1 lub FAILURE gdy SIGUSR2
    // calosc powtarza sie r razy
    int pid;
    int noChildren = 0;

    sethandler(info, SIGUSR1);
    sethandler(info, SIGUSR2);

    for (int i = 0; i < r; i++) {
        printf("Creation of children number: %d\n", i+1);
        for (int j = 0; j < n; j++) {
            if ((pid = fork()) == 0) { // child
                break;
            } else if (pid > 0) { // parent
                noChildren++;
            } else {
                ERR("pid");
            }
        }
        if (pid == 0) { // child
            srand(time(NULL) ^ getpid());
            int sleepTime = rand() % 5 + 5;
            // nanosleep
            struct timespec req = {sleepTime, 0}; 
            struct timespec rem; // Tu system wpisze, ile czasu zostało
            while (nanosleep(&req, &rem) == -1 && errno == EINTR) {
                if (lastsig == SIGUSR1) printf("SUCCESS\n");
                else if (lastsig == SIGUSR2) printf("FAILURE\n");
                req = rem; 
            }
            exit(EXIT_SUCCESS);
            // sleep
            // while (sleepTime = sleep(sleepTime)) {
            //     if (lastsig == SIGUSR1) printf("SUCCESS");
            //     else if (lastsig == SIGUSR2) printf("FAILURE");
            // }
        }
        
        if (pid > 0) { // parent
            while (1) {
                if (waitpid(0, NULL, WNOHANG)) noChildren--;
                if (noChildren == 0) break;
                sleep(k);
                kill(0, SIGUSR1);
                sleep(p);
                kill(0, SIGUSR2);
            }
        }
    }
}