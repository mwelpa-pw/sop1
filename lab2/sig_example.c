#include <stdio.h>
#include <signal.h>
#include <unistd.h> // For getpid()

// Signal handler function
void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        printf("Received SIGUSR1 signal!\n");
    } else if (signum == SIGINT) {
        printf("Received SIGINT signal. Exiting.\n");
        _exit(0); // Safely exit the process
    }
}

int main() {
    struct sigaction sa;

    // Configure the signal handler
    sa.sa_handler = signal_handler; // Set our handler function
    sigemptyset(&sa.sa_mask);       // Clear all other signals from the mask
    sa.sa_flags = 0;                // No special flags

    // Register the signal handler for SIGUSR1
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Error registering SIGUSR1 handler");
        return 1;
    }

    // Register the same handler for SIGINT (Ctrl+C)
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("Error registering SIGINT handler");
        return 1;
    }

    printf("My PID is %d\n", getpid());
    printf("Waiting for SIGUSR1 or SIGINT (Ctrl+C). Try: kill -USR1 %d\n", getpid());

    // Keep the program running indefinitely, waiting for signals
    while (1) {
        sleep(1); // Sleep to prevent busy-waiting
    }

    return 0;
}
