#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define NOM_ARCHIVO "Google.html"
#define PUERTO 9999

// Función que inicializa el servidor y devuelve el conector del socket
int iniciarServidor(int puerto) {
    int conector;
    struct sockaddr_in servidor;

    conector = socket(AF_INET, SOCK_STREAM, 0);

    servidor.sin_family = AF_INET; 
    servidor.sin_port = htons(puerto); 
    servidor.sin_addr.s_addr = INADDR_ANY; 

    bind(conector, (struct sockaddr *)&servidor, sizeof(servidor));
    listen(conector, 5); 

    return conector;
}

// Función que atiende al cliente y le envia el archivo solicitado
void atenderElCliente(int connector_cliente, char *nom_archivo) {
    int aux_archivo = open(nom_archivo, O_RDONLY);
    int leidos;
    char buffer[1024];
    
    while((leidos = read(aux_archivo, buffer, 1024)) > 0) {
        write(connector_cliente, buffer, leidos);
    }
    
    close(aux_archivo);
    close(connector_cliente);
    exit(0);
}

// Función principal del servidor 
int main() {
    int conector = iniciarServidor(PUERTO);
    
    while(1) {
        int connector_cliente = accept(conector, NULL, NULL);
        
        pid_t pid = fork();
        
        if(pid == 0) {
            close(conector);
            
            atenderElCliente(connector_cliente, NOM_ARCHIVO);
        }
        if(pid > 0) {
            close(connector_cliente);
        }
    }
    
    return 0;
}