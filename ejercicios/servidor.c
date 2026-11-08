#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char *nom_archivo = argv[1];
    int conector;
    struct sockaddr_in servidor;
    servidor.sin_family=AF_INET; //el socket comunica a traves de la red de internet
    servidor.sin_port=htons(9999); // la puerta reservada 
    servidor.sin_addr.s_addr=INADOR_ANY; // cualquier direccion de la maquina

    conector=socket(AF_INET, SOCK_STREAM,0);

    bind(conector, (struct sockaddr *)&servidor, sizeof(servidor));
    listen(conector, 5); // el tamanio maximo de la cola es 5 , pero hay que preguntar si puedo poner SOMAXCONN para que so asigna auticamente 
    while(1){
        int connector_cliente= accept(conector, NULL, NULL);
        
        pid_t pid = fork();
        if(pid==0){
            close (conector);
            int aux_archivo= open(nom_archivo, O_RDONLY);
            int leidos;
            char buffer[1024];
            while((leidos=read(aux_archivo, buffer,1024))>0){
                write(conector_cliente, buffer, leidos);
            }
            close(aux_archivo);
            close(connector_cliente);
            exit(0);
        }
        if(pid>0){
            close(conector_cliente);
        }
    }
    

}