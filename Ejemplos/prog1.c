#include <stdio.h>
#include <unistd.h>
#include <string.h> // Necesario para strcpy
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int i;
    printf("\nEjecutando el programa invocador (prog1). Sus argumentosson: \n");
    for (i = 0; i < argc; i++)
        printf(" argv[%d] : %s \n", i, argv[i]);
    sleep(10);
    strcpy(argv[0], "prog2");
    // Con strcpy(argv[0], "prog2");, sobreescribe el primer argumento
    //(el nombre del programa actual) y lo cambia por el texto "prog2".
    // Hace esto para que, cuando llame al siguiente programa, este crea que fue invocado con el nombre "prog2".
    if (execvp("./prog2", argv) < 0)
    // La función execvp busca un archivo ejecutable llamado prog2 en el directorio actual (
    //./) y le pasa la lista de argumentos (argv) que acaba de modificar.
    {
        printf("Error en la invocacion a prog2 \n");
        exit(1);
    };
    exit(0);
}

// Lo que hace exec no es abrir un programa nuevo y esperar a que termine, sino que reemplaza por completo
// la memoria y el código del programa actual (prog1) por el código del nuevo programa (prog2).