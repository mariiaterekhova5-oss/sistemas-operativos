#include <stdio.h>  // Para printf y perror
#include <unistd.h> // Para getpid
#include <signal.h> // Para kill y SIGKILL

int main()
{
    printf("Voy a suicidarme\n");
    // getpid() obtiene el ID del proceso actual
    // SIGKILL (señal 9) lo destruye instantáneamente
    kill(getpid(), SIGKILL);
    perror("No he muerto???");

    return 0;
}