#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    char *nom_archivo = argv[1];
    int conector;
    struct sockaddr_in servidor;
    conector=socket(AF_INET, SOCK_STREAM,0);
    bind(conector, addr, addlen);
    listen(conector, 5); // el tamanio maximo de la cola es 5 , pero hay que preguntar si puedo poner SOMAXCONN para que so asigna auticamente 
    while(1){
        accept(conector, addr, addlen);
        fork();
        pid_t pid = getpid();
        if(pid==0){
            int aux_archivo= open(nom_archivo, O_RDWR);
            int leidos;
            char buffer[1024];
            while((leidos=read(aux_archivo, buffer,1024))>0){
                write(conector, buffer, leidos);
            }
            close(aux_archivo);
            exit(0);
        }
        if(pid>0){
            close(conector);
        }
    }
    

}