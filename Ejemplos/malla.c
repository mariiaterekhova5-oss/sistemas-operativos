#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);

    for (int j = 0; j < y; j++)
    {
        pid_t pid = fork();
        for (int i = 1; i < x; i++)
        {
            pid_t child_pid = fork();
            if (child_pid > 0)
            {               // Si es el padre de este eslabón de la cadena
                wait(NULL); // Espera a que su subhijo termine para no destruir el árbol[cite: 1]
                exit(0);    // Cuando el subhijo muere, este padre también muere ordenadamente
            }
            else if (child_pid < 0)
            {
                perror("Error en fork");
                exit(1);
            }
        }
    }
}