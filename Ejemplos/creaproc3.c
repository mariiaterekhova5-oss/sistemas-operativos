#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
// Nota: en sistemas modernos suele hacer falta incluir <sys/wait.h> para usar wait()

int main()
{
    int estado, numero;
    // no creamos aqui ni fork ni pid , ya que en switch se crea directamente fork
    switch (fork())
    {
    case -1: /* ERROR */
        perror("Error en fork");
        exit(1);
    case 0: /* HIJO */
        numero = 13;
        printf("Soy el hijo y muero con %d...\n", numero);
        sleep(20);
        exit(numero);
        // acaba y tiene el resultado del codigo 13
        // El sistema operativo no mete el "13" tal cual en la variable estado. Empaqueta mucha información
    default: /* PADRE */
        wait(&estado);
        // Bloquea al padre. El padre se queda ahí congelado esperando a que el hijo muera.
        printf("Soy el padre. ");
        // Cuando el hijo muere a los 20 segundos, el sistema operativo coge ese 13 que dejó el hijo y lo guarda dentro de la variable estado del padre.
        if ((estado & 0x7F) != 0)
        {
            printf("Mi hijo ha muerto con una señal.\n");
            // Esto revisa los primeros 7 bits. Si hay algo distinto de cero ahí, significa que el hijo no murió de forma natural, sino que alguien lo mató "a la fuerza"
        }
        else
        {
            printf("Mi hijo ha muerto con exit(%d).\n", (estado >> 8) & 0xFF);
            // Si el hijo murió de forma natural, el número que puso en su exit() (el 13) está escondido a partir del bit número 8. Lo que hace el código aquí es desplazar los bits 8 posiciones a la derecha (>> 8) para "desempaquetar" ese número y poder imprimirlo en el printf.
        }
        exit(0);
    }
}