#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
// cuando arranco el programa , se crea el padre
int main()
{
    pid_t pid;
    // se crea un caja vacia en la memoria
    pid = fork(); // crea un clon del pid que se llama hijo

    switch (pid)
    {
    case -1:
        printf("No he podido crear el proceso hijo \n");
        break;
    case 0:
        // el sistema usa 0 como una señal que nos enseña que hemos creado un hijo
        printf("Soy el hijo, mi PID es %d y mi PPID es %d \n", getpid(), getppid());
        sleep(20); // pausa la ejecución de ese proceso durante 20 segundos. El programa se queda "congelado" en esa línea y no avanza a la siguiente hasta que pase ese tiempo.
        break;
    default:
        printf("Soy el padre, mi PID es %d y el PID de mi hijo es %d \n", getpid(), pid);
        sleep(30);
        // si pid es de pid>0 , me devuelva el resultado de default
    }
    printf("Final de ejecución de %d \n", getpid());
    exit(0);
}