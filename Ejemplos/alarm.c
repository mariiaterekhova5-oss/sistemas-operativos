#include <stdio.h>  // Necesario para printf
#include <unistd.h> // Necesario para alarm
#include <signal.h> // Necesario para signal y SIGALRM

// IMPORTANTE: Se añade 'volatile' para variables modificadas por señales
volatile int seguir = 1;

void fin(int n)
{
    seguir = 0; // Cambia el interruptor a 0 para detener el bucle
}

int main()
{
    int contador = 0;

    signal(SIGALRM, fin);
    alarm(5);
    // Pasados los 5 segundos, el sistema operativo le dispara la señal SIGALRM al programa
    do
    {
        printf("Esta es la línea %d\n", contador++);
    } while (seguir);

    printf("TOTAL: %d líneas\n", contador);

    return 0;
}