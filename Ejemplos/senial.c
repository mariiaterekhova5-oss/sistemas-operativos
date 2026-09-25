#include <stdio.h>  // Necesario para printf
#include <unistd.h> // Necesario para alarm y pause
#include <signal.h> // Necesario para signal y SIGALRM

// Las funciones manejadoras de señales deben aceptar un argumento entero
void alarma(int sig)
{
    printf("acabo de recibir un SIGALRM\n");
}

int main() // main siempre debe devolver un int en C moderno
{
    signal(SIGALRM, alarma);
    // Modifica el comportamiento por defecto del programa frente a una señal.
    // Le indica al sistema operativo: "Si recibes la señal SIGALRM (señal de alarma temporizada),
    // no termines el programa; en su lugar, ejecuta la función alarma que he definido".

    printf("Acabo de programar la captura de un SIGALRM\n");

    alarm(3);
    // Ordena al sistema que envíe una señal SIGALRM
    // a este mismo proceso una vez que transcurran exactamente 3 segundos.
    printf("Ahora he programado la alarma en 3 seg.\n");

    pause(); // El programa se detiene aquí hasta recibir una señal

    printf("Ahora continúo con la ejecución normal\n");

    return 0;
}