#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define PUERTO 9999
#define TAM_BUFFER 1024

//funcion que conecta el cliente con el servidor y devuelve el conector del socket
int conectarServidor(char *ip_destino){
    int conector;
    struct sockaddr_in servidor;

    conector = socket(AF_INET, SOCK_STREAM, 0);

    servidor.sin_family=AF_INET;
    servidor.sin_port=htons(PUERTO);
    servidor.sin_addr.s_addr=inet_addr(ip_destino);

    connect(conector, (struct sockaddr *)&servidor, sizeof(servidor));
    return conector;
}

//funcion que recibe el archivo del servidor y lo imprime en la pantalla
void recibirArchivo(int conector){
    char buffer[TAM_BUFFER];
    int leidos;

    while((leidos=read(conector, buffer, TAM_BUFFER))>0){
        write(1, buffer, leidos);

    }
}

//funcion principal del cliente 
int main(int argc, char *argv[]){
    char *ip_destino= argv[1];
    
    int conector=conectarServidor(ip_destino);
    recibirArchivo(conector);
    
    close(conector);

}