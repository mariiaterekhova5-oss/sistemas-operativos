#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void manejador_A(int sig) {
    printf("/*Resultado del comando \"pstree\" */\n");

    system("pstree -c"); 
}

void manejador_Z(int sig) {
}

int main(int argc, char *argv[]) {
    int tiempo= atoi(argv[1]);
    pid_t pid_ejec= getpid();
    printf("Soy el proceso ejec: mi pid es %d\n", pid_ejec);
    pid_t pid_A= fork();
    if (pid_A ==0) {
        pid_t num_pid_A=getpid();
        printf("Soy el proceso A: mi pid es %d. Mi padre es %d\n", num_pid_A, pid_ejec);
        signal(SIGUSR1, manejador_A);
        pid_t pid_B = fork();

        if(pid_B==0) {
            pid_t num_pid_B=getpid();
            printf("Soy el proceso B: mi pid es %d. Mi padre es %d. Mi abuelo es %d\n", num_pid_B, num_pid_A, pid_ejec);
            for(int i=0; i<3; i++){
                pid_t pid= fork();
                if(pid==0){
                    switch (i){
                        case 0:
                            printf("Soy el proceso X: mi pid es %d.Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n", getpid() ,num_pid_B, num_pid_A, pid_ejec);
                            sleep(tiempo+ 3);
                            printf("Soy X(%d) y muero\n", getpid());
                        break;
                        case 1:
                            printf("Soy el proceso Y: mi pid es %d.Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n", getpid() ,num_pid_B, num_pid_A, pid_ejec);
                            sleep(tiempo+ 2);
                            printf("Soy Y(%d) y muero\n", getpid());
                        break;
                        case 2:
                            printf("Soy el proceso Z: mi pid es %d.Mi padre es %d. Mi abuelo es %d. Mi bisabuelo es %d\n", getpid() ,num_pid_B, num_pid_A, pid_ejec);
                            signal(SIGALRM, manejador_Z); 
                            alarm(tiempo); 
                            pause();     
                            
                            kill(num_pid_A, SIGUSR1); 
                            
                            sleep(1); 
                            printf("Soy Z (%d) y muero\n", getpid());
                        break;
                    }
                    exit(0);
                }
            }
            wait(NULL); wait(NULL); wait(NULL);
            printf("Soy B(%d) y muero", num_pid_B);
            exit(0);

        }
        else if (pid_B>0){
            wait(NULL); 
            printf("Soy A(%d) y muero",num_pid_A );
            exit(0);
        }
    }
    else if (pid_A>0){
        wait(NULL); 
        printf("Soy ejec (%d) y muero",pid_ejec );
        exit(0);
    }
    return 0;
}
    

