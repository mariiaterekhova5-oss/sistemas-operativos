#include <stdio.h>  // Para printf y perror
#include <stdlib.h> // Para exit y atol
#include <unistd.h> // Para read y close
#include <fcntl.h>  // Para open y O_RDONLY

int main()
{
    int df, tam;
    long numero;
    char buffer[10];

    //  Abrimos el terminal actual en modo solo lectura
    df = open("/dev/tty", O_RDONLY);
    if (df < 0)
    {
        perror("Error al abrir tty");
        exit(-1);
    }

    // Leemos lo que el usuario escriba por teclado
    tam = read(df, buffer, 9); /* COMO MUCHO DE HASTA 9 DIGITOS */

    if (tam == -1)
    {
        perror("Error de lectura");
    }
    else
    {
        // Convertimos los datos leídos en una cadena de texto válida
        buffer[tam] = '\0'; /* PONE EL FINAL DE CADENA (mejor usar '\0' que 0) */

        // Convertimos el texto a número
        numero = atol(buffer); // Usamos atol en lugar de atoi para tipo long
        printf("Resultado: %ld\n", numero * 2);
    }
    close(df);

    return 0;
}