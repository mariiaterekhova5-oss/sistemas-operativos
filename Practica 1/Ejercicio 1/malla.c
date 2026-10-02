#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("Argumentos incorrectos\n");
        exit(1);
    }

    int x = atoi(argv[1]);
    int y = atoi(argv[2]);

    pid_t pid_raiz = getpid(); 

    for (int j = 0; j < y; j++)
    {
        pid_t pid = fork(); 
        
        if (pid == 0) { 
            
            for (int i = 1; i < x; i++)
            {
                pid_t child_pid = fork();
                
                if (child_pid > 0)
                {               
                    
                    wait(NULL); 
                    exit(0);    
                }
                else if (child_pid < 0)
                {
                    perror("Error en fork");
                    exit(1);
                }
                
            }
            sleep(1);
            exit(0);
        }
        else if (pid < 0) {
            perror("Error en fork");
            exit(1);
        }
           
    }


    sleep(1);

    
    char pid_str[16];
    sprintf(pid_str, "%d", pid_raiz);

    if (fork() == 0) {
        
        execlp("pstree", "pstree", "-c", pid_str, NULL);
        perror("Error al ejecutar pstree");
        exit(1);
    }

    
    wait(NULL);

    for (int j = 0; j < y; j++) {
        wait(NULL);
    }

    return 0;
}