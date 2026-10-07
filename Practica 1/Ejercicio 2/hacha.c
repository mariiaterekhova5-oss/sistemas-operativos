#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>   // Soluciona los errores de open, O_RDONLY y creat
#include <string.h>

int main(int argc, char *argv[]) { 

    char *nombre_archivo = argv[1]; 

    int tamano_trozo = atoi(argv[2]);

    int fd_original = open(nombre_archivo, O_RDONLY);
    if (fd_original < 0) {
        perror("Error al abrir el archivo original");
        return 1; 
    }

    
    long tamano_total = lseek(fd_original, 0, SEEK_END); 
    
    
    lseek(fd_original, 0, SEEK_SET);

    
    int hijos = tamano_total / tamano_trozo;

    if (tamano_total % tamano_trozo != 0) {
        hijos++;
    }

    

    for (int i = 0; i < hijos; i++) {
        int fd_tuberia[2];
        if (pipe(fd_tuberia) == -1) {
            perror("Error al crear la tubería");
            return 1; 
        }

        pid_t pid = fork();

        if (pid == 0) { 
            
            close(fd_tuberia[1]); 
            
            char nombre_destino[256];
            
            strcpy(nombre_destino, nombre_archivo); 
            
            int len = strlen(nombre_destino); 
            
            
            nombre_destino[len] = '.'; 
            nombre_destino[len + 1] = 'h'; 
            
            
            nombre_destino[len + 2] = (i / 10) + '0'; 
            nombre_destino[len + 3] = (i % 10) + '0'; 
             
            
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
        else  { 
            char informacion[tamano_trozo];
            int leidos = read(fd_original, informacion, tamano_trozo); 
            if (leidos > 0) { 
                write(fd_tuberia[1], informacion, leidos); 
            }
            close(fd_tuberia[1]);
            

            close(fd_tuberia[1]);
            
            wait(NULL);
        }
    } 

    close(fd_original); 
    return 0;
}