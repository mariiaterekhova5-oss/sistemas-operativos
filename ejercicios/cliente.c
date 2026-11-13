#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h> 

#define PUERTO 9999
#define TAM_BUFFER 1024

// Función que comprueba errores de llamadas al sistema
void manejo_errores(int variable, const char *mensaje) {
    if (variable < 0) {
        perror(mensaje);
        exit(1);
    }
}

// Función auxiliar: Valida que el usuario introduzca la IP al ejecutar
void validarArgumentos(int argc) {
    if (argc != 2) {
        printf("Uso correcto: Cliente IP_Servidor\n");
        exit(1);
    }
}

// Función auxiliar: Valida que el formato de la IP tenga sentido usando struct sockaddr_in
void validarFormatoIP(struct sockaddr_in servidor, int conector) {
    if (servidor.sin_addr.s_addr == INADDR_NONE) {
        printf("Error: La dirección IP \n");
        close(conector);
        exit(1);
    }
}

// Función que conecta el cliente con el servidor y devuelve el conector del socket
int conectarServidor(char *ip_destino){
    int conector;
    struct sockaddr_in servidor;

    conector = socket(AF_INET, SOCK_STREAM, 0);
    manejo_errores(conector, "Error al crear el socket");

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(PUERTO);
    servidor.sin_addr.s_addr = inet_addr(ip_destino);

    // Le pasamos la estructura entera, que sí está en vuestro temario
    validarFormatoIP(servidor, conector);

    int aux_connect = connect(conector, (struct sockaddr *)&servidor, sizeof(servidor));
    manejo_errores(aux_connect, "Error al conectar (¿Está el servidor encendido?)");
    
    return conector;
}

// Función que recibe el archivo del servidor y lo imprime en la pantalla
void recibirArchivo(int conector){
    char buffer[TAM_BUFFER];
    int leidos;

    while((leidos=read(conector, buffer, TAM_BUFFER))>0){
        int escritos = write(1, buffer, leidos);
        manejo_errores(escritos, "Error al escribir en la pantalla");
    }
    
    manejo_errores(leidos, "Error al leer datos del servidor");
}

// Función principal del cliente 
int main(int argc, char *argv[]){
    validarArgumentos(argc);

    char *ip_destino = argv[1];
    
    int conector = conectarServidor(ip_destino);
    recibirArchivo(conector);
    
    close(conector);
    return 0; 
}