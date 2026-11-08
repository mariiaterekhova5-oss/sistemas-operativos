#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

//funcion que inicializa el servidor y devuelve el conector del socket
int iniciarServidor(int puerto){
    int conector;
    struct sockaddr_in servidor;

    servidor.sin_family=AF_INET; //el socket comunica a traves de la red de internet
    servidor.sin_port=htons(puerto); // la puerta reservada 
    servidor.sin_addr.s_addr=INADOR_ANY; // cualquier direccion de la maquina

    conector=socket(AF_INET, SOCK_STREAM,0);

    bind(conector, (struct sockaddr *)&servidor, sizeof(servidor));
    listen(conector, 5); // el tamanio maximo de la cola es 5 , pero hay que preguntar si puedo poner SOMAXCONN para que so asigna auticamente 

    return conector;
}

//funcion que atiende al cliente y le envia el archivo solicitado
void atenAtenderElCliente(int connector_cliente, char *nom_archivo){
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

//funcion principal del servidor 
int main(int argc, char *argv[]) {
    char *nom_archivo = argv[1];
    int conector=iniciarServidor(9999);
    while(1){
        int connector_cliente= accept(conector, NULL, NULL);
        
        pid_t pid = fork();
        if(pid==0){
            close (conector);
            atenAtenderElCliente(connector_cliente, nom_archivo);
        }
        if(pid>0){
            close(conector_cliente);
        }
    }
    

}