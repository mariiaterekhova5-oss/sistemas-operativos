#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NOM_ARCHIVO "Google.html"
#define PUERTO 9999
#define TAM_BUFFER 1024

// Funcion que comprueba errores de forma compacta
void manejo_errores(int variable, const char *mensaje) {
    if (variable < 0) {
        perror(mensaje);
        exit(1);
    }
}

//  Funcion que atiende a un cliente, enviandole el archivo solicitado
void atenderElCliente(int connector_cliente, char *nom_archivo) {
    int aux_archivo = open(nom_archivo, O_RDONLY);
    manejo_errores(aux_archivo, "Error al abrir el archivo");

    int leidos;
    char buffer[TAM_BUFFER];
    
    while ((leidos = read(aux_archivo, buffer, TAM_BUFFER)) > 0) {
        int escritos = write(connector_cliente, buffer, leidos);
        manejo_errores(escritos, "Error al enviar al cliente");
    }
    
    manejo_errores(leidos, "Error al leer el archivo");
    
    close(aux_archivo);
    close(connector_cliente);
    exit(0);
}

// Funcion que delega la atencion de un cliente a un proceso hijo
void delegarCliente(int conector_principal, int connector_cliente) {
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Error en fork");
        close(connector_cliente);
    } 
    else if (pid == 0) {
        close(conector_principal);
        atenderElCliente(connector_cliente, NOM_ARCHIVO);
    } 
    else if (pid > 0) {
        close(connector_cliente);
    }
}

// Funcion que inicia el servidor 
int iniciarServidor(int puerto) {
    int conector;
    struct sockaddr_in servidor;

    conector = socket(AF_INET, SOCK_STREAM, 0);
    manejo_errores(conector, "Error al crear el socket");

    servidor.sin_family = AF_INET; 
    servidor.sin_port = htons(puerto); 
    servidor.sin_addr.s_addr = INADDR_ANY; 

    int aux1 = bind(conector, (struct sockaddr *)&servidor, sizeof(servidor));
    manejo_errores(aux1, "Error en bind");

    int aux2 = listen(conector, 5);
    manejo_errores(aux2, "Error en listen");

    return conector;
}

// Funcion que gestiona las conexiones entrantes de clientes
void gestionarConexiones(int conector) {
    while (1) {
        int connector_cliente = accept(conector, NULL, NULL);

        if (connector_cliente < 0) {
            perror("Error en accept");
        } 
        else {
            delegarCliente(conector, connector_cliente);
        }
    }
}

// Función principal 
int main() {
    int conector = iniciarServidor(PUERTO);
    gestionarConexiones(conector);
    close(conector);
    return 0;
}