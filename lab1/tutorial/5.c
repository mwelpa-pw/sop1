#include <stdio.h>    // Do fprintf i stderr
#include <stdlib.h>   // Do EXIT_SUCCESS
#include <unistd.h>   // Do sleep (na systemach POSIX)

int main()
{
    for (int i = 0; i < 15; ++i)
    {
        // Używamy fprintf, kierując wyjście na strumień stderr
        fprintf(stderr, "Iteracja: %d", i); 
        
        // Znak nowej linii ('\n') wymusza opróżnienie bufora, 
        // dzięki czemu treść jest widoczna natychmiast, co jest kluczowe 
        // przy użyciu sleep().
        
        sleep(1);
    }
    return EXIT_SUCCESS;
}