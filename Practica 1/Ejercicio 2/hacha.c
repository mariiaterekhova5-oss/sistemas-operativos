
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h> // Necesaria para strcpy y strlen

int main(int argc, char *argv[]) { // Corregido a char *argv[]

    char *nombre_archivo = argv[1]; // Corregido a char *

    int tamano_trozo = atoi(argv[2]);

    int fd_original = open(nombre_archivo, O_RDONLY);
    if (fd_original < 0) {
        perror("Error al abrir el archivo original");
        return 1; // Sustituido exit por return
    }

    // Desplazamos el cursor al final (Corregido SEEEK_END a SEEK_END)
    long tamano_total = lseek(fd_original, 0, SEEK_END); 
    
    // Lo devolvemos al principio
    lseek(fd_original, 0, SEEK_SET);

    // Corregido tamanio_total y tamano_troza
    int hijos = tamano_total / tamano_trozo;

    if (tamano_total % tamano_trozo != 0) {
        hijos++;
    }

    int fd_tuberia[2];

    for (int i = 0; i < hijos; i++) {

        if (pipe(fd_tuberia) == -1) {
            perror("Error al crear la tubería");
            return 1; // Sustituido exit por return
        }

        pid_t pid = fork();

        if (pid == 0) { // hijo
            
            close(fd_tuberia[1]); // Por seguridad, cierra su boca de escritura
            
            char nombre_destino[256];
            
            strcpy(nombre_destino, nombre_archivo); 
            
            int len = strlen(nombre_destino); 
            
            // 3. Añadimos la extensión ".h" manualmente en las siguientes posiciones 
            nombre_destino[len] = '.'; // Separado del comentario
            nombre_destino[len + 1] = 'h'; 
            
            // 4. Calculamos los dos dígitos del número 'i' y los convertimos a texto
            nombre_destino[len + 2] = (i / 10) + '0'; 
            nombre_destino[len + 3] = (i % 10) + '0'; 
             
            // 5. ¡Obligatorio en C! Cerramos la cadena
            nombre_destino[len + 4] = '\0';
            
            int fd_destino = creat(nombre_destino, 0666);
            
            if (fd_destino < 0) {
                perror("Error al crear el archivo de destino");
                return 1;
            }
            
            char buffer[1024]; 
            int leidos; 
            
            while ((leidos = read(fd_tuberia[0], buffer, sizeof(buffer))) > 0) { 
                 write(fd_destino, buffer, leidos); 
            }

            close(fd_tuberia[0]);
            close(fd_destino);
            return 0; 
            
        } 
        else if (pid > 0) { // padre
            
            close(fd_tuberia[0]); 

            char buffer[1024];
            int leidos; 
            int restantes = tamano_trozo; 
            
            while (restantes > 0) { 
                int a_leer = (restantes < sizeof(buffer)) ? restantes : sizeof(buffer); 
                
                leidos = read(fd_original, buffer, a_leer); 
                
                if (leidos <= 0) { 
                    break; 
                } 
                 
                write(fd_tuberia[1], buffer, leidos); 
                restantes = restantes - leidos; 
            }

            close(fd_tuberia[1]);
            
            wait(NULL);
        }
    } 

    close(fd_original); 
    return 0;
}