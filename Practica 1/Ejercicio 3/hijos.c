#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s x y\n", argv[0]);
        return 1;
    }

    int x = atoi(argv[1]);
    int y = atoi(argv[2]);

    int shmid_padres = shmget(IPC_PRIVATE, x * sizeof(int), IPC_CREAT | 0666);
    int shmid_hijos  = shmget(IPC_PRIVATE, y * sizeof(int), IPC_CREAT | 0666);

    int *padres = (int *) shmat(shmid_padres, NULL, 0);
    int *hijos_finales = (int *) shmat(shmid_hijos, NULL, 0);

    for (int i = 0; i < x; i++) {
        
        padres[i] = getpid();
        
        if (i < x - 1) {
            pid_t pid = fork();
            
            if (pid > 0) { 
                wait(NULL); 
                
                if (i == 0) { 
                    printf("Soy el superpadre (%d) : mis hijos finales son: ", getpid());
                    for(int j = 0; j < y; j++) {
	                    if(j==y-1){
		                    printf("%d", hijos_finales[j]);
	                    }else{
		                    printf("%d, ", hijos_finales[j]);
	                    }
                        
                    }
                    printf("\n");

                    shmctl(shmid_padres, IPC_RMID, NULL);
                    shmctl(shmid_hijos, IPC_RMID, NULL);
                }
                
                shmdt(padres);
                shmdt(hijos_finales);
                return 0; 
            }
        }
    }

    for (int j = 0; j < y; j++) {
        pid_t pid_hoja = fork();
        hijos_finales[j] = getpid(); 
        if (pid_hoja == 0) {
            printf("Soy el subhijo (%d), mi padres son: ", getpid());
            for(int k = 0; k < x; k++) {
                if(k==x-1){
		            printf("%d", padres[k]);
	            }else{
		            printf("%d, ", padres[k]);
	            }
            }
            printf("\n");
            shmdt(padres);
            shmdt(hijos_finales);
            return 0; 
        }
    }
    for (int j = 0; j < y; j++) {
        wait(NULL);
    }
    shmdt(padres);
    shmdt(hijos_finales);
    return 0;
}
