#include <stdio.h>  // Para perror
#include <stdlib.h> // Para exit
#include <unistd.h> // Para write, close
#include <fcntl.h>  // Para creat
#include <string.h> // Para strcpy, strcat, strlen

// env[] es un arreglo especial que recibe las variables de entorno del sistema
int main(int argc, char *argv[], char *env[])
{
    int df, cont = 0;

    // Aumentamos a 4096 para evitar un desbordamiento de memoria (Buffer Overflow)
    char buffer[4096];

    // Crea (o sobreescribe) el archivo con permisos 0755 (lectura/escritura/ejecución)
    df = creat("variables.txt", 0755);

    if (df < 0)
    {
        perror("Error al crear archivo");
        exit(-1);
    }
    while (env[cont] != NULL)
    {

        strcpy(buffer, env[cont]);

        strcat(buffer, "\n");

        // Escribimos el contenido exacto del buffer dentro del archivo
        if (write(df, buffer, strlen(buffer)) != strlen(buffer))
        {
            perror("Error al escribir");
            break; /* Si algo falla al escribir, salimos del bucle */
        }

        cont++; // Pasamos a la siguiente variable
    }

    close(df);

    return 0;
}