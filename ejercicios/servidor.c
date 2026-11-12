#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>

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

//Funcion de manejador para recoger a los procesos hijos y evitar zombies
void manejadorZombies(int senial) {
    wait(NULL);
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
    
    // Si read devuelve -1, también saltará un error
    manejo_errores(leidos, "Error al leer el archivo");
    
    close(aux_archivo);
    close(connector_cliente);
    exit(0);
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
    signal(SIGCHLD, manejadorZombies);

    while (1) {
        int connector_cliente = accept(conector, NULL, NULL);
        
        // Aquí NO usamos manejo_errores() porque si falla un cliente 
        // no queremos hacer exit(1) y matar el servidor entero. //hay mencionarlo en la memoria
        if (connector_cliente < 0) {
            perror("Error en accept");
            continue;
        }
        
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("Error en fork");
            close(connector_cliente);
            continue;
        } 
        else if (pid == 0) {
            close(conector);
            atenderElCliente(connector_cliente, NOM_ARCHIVO);
        } 
        else if (pid > 0) {
            close(connector_cliente);
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