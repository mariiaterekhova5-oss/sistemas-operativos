#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char *argv[]) {
    char *nombre_archivo= argv[1];
    int tamano_trozo=atoi(argv[2]);

    struct stat info_archivo;

    long tamano_total= info_archivo.st_size;

    int hijos= tamano_total/tamano_trozo;

    if(tamano_total%tamano_trozo!=0){
        hijos++;
    }

    
}