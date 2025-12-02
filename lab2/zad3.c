//int sigsuspend(const sigset_t *sigmask); // zatrzymanie programu do momentu, gdy otrzyma on
//      sygnał. OUT: -1 Błąd, 
//
// sigsuspend zawiesza program, az ten dostanie sygnal z maski w sigset
// proces obudza sygnaly ktorych nie ma w masce

// const sigset_t maska;
//int sigemptyset(maska);
//int sigaddset(sigset_t *set, int signo); -> Sig suspend przespi ten sygnal.
//              maska           SIGUSR1

//int sigprocmask(int how, const sigset_t *restrict set,
//          sigset_t *restrict oset);
// SIG_BLOCK -> +
// SIG_SETMASK -> 1
// SIG_UNBLOCK -> okresla co chcemy usunac 
// set typu const sigset_t to wskaźnik na maskę, na podstawie której chcemy
//       modyfikować poprzednią maskę.
// oset typu sigset_t * jest wskaźnikiem na obiekt do którego chcemy zapisać
//       maskę sygnałów sprzed edycji.

// 1. Tworzysz pusta maske
// 2. Dodajesz sygnal na ktory chce czekac
// 3. sigprocmask() i zapisuje stara
// 4. sigsuspend(stara_maska) => wtedy moge juz odczytywac sygnal
//          Musze ustawic handler!!!

