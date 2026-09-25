#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    int i;
    int j;

    pid_t pid;
    // reservamos tres  cajas en menoria - padre , y dos variabjes
    pid = fork();
    // tenemos dos procesos en la memoria , variables i y j
    switch (pid)
    {
    case -1:
        printf("\nNo he podido crear el proceso hijo");
        break;
    case 0:
        // el hijo
        i = 0;
        printf("\nSoy el hijo, mi PID es %d y mi variable i (inicialmente a %d) es par", getpid(), i);
        for (j = 0; j < 5; j++)
        {
            i++;
            i++;
            printf("\nSoy el hijo, mi variable i  es %d", i);
            // el hijo va imprimiendo por pantalla los números pares: 2, 4, 6, 8, 10.
        };
        break;
    default:
        i = 1;
        printf("\nSoy el padre, mi PID es %d y mi variable i (inicialmente a %d) es impar", getpid(), i);
        for (j = 0; j < 5; j++)
        {
            i++;
            i++;
            printf("\nSoy el padre, mi variable i  es %d", i);
            // el hijo va imprimiendo por pantalla los números impares
        };
    }
    printf("\nFinal de ejecucion de %d \n", getpid());
    exit(0);
}