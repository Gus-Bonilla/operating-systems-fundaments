#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define TAM_CONT 100
#define pid_length 4

void sig_handler(int sig_kill);

int arraypid[6];

int main()
{	
	int pid;
	int i;
	int pid_padre;
	FILE *f_sh;
	
	signal(SIGUSR1,sig_handler);

	// Vacía el contenido de PIDs_GETTY
	f_sh=fopen("PIDs_GETTY","w+");
	fclose(f_sh);
	
	// Poner mi PID en un archivo
	f_sh=fopen("PID_INIT","w");
	fprintf(f_sh,"%d",getpid());
	fclose(f_sh);
    
    printf("\nPID_INIT (PROCESO PADRE): %d\n",getpid());
	
	for(i=0;i<6;i++)
	{
		pid = fork();
		
		if(pid == 0)
		{
			printf("\nPID del proceso%d, %d\n",i,getpid());
			execlp("xterm","xterm","-e","./getty",NULL);
		}	
	}

	while(1)
	{
		int killed_term;
        int killed_start=0;

        killed_term = wait(NULL);
        // Para este punto se cerró una consola

        f_sh=fopen("PIDs_GETTY","r");
		char contenido[TAM_CONT];
		char array[pid_length+1];
		int longitud;
		int pid_kill[6];

	    array[pid_length]=0;     // Es necesario para que el atoi identifique el fin de la cadena
		longitud=fread(contenido,1,pid_length*6,f_sh);
        fclose(f_sh);

        for(int i=0;i<6;i++)
	   { 
         	for(int idx=0;idx<pid_length;idx++)
			 {
				 array[idx]=contenido[(i*pid_length)+idx];
			 }

			pid_kill[i] = atoi(array);

			printf("\n%d\n", pid_kill[i]);

			if(pid_kill[i]==killed_term){
				killed_start = i;
			}	 	
		}

		pid = fork();

		if(pid==0){
			printf("\nPID del proceso nuevo: %d\n",getpid());
			execlp("xterm","xterm","-e","./getty",NULL);
		
		}else{
			//char k[pid_length];
    		//sprintf(k, "%i", pid);

    		//for(int adx=0; adx<pid_length; adx++){
    		//	contenido[adx+killed_start] = k[adx];
    		//}

			f_sh=fopen("PIDs_GETTY","w+"); // LAMMAR A FUNCION DE BORRADO
    		//fprintf(f_sh,"",NULL);
			fclose(f_sh);
			
			f_sh=fopen("PIDs_GETTY","a");

			for(int i=0; i<6; i++){
				if(pid_kill[i] == killed_term)
					fprintf(f_sh,"%d", pid);
				else
					fprintf(f_sh,"%d", pid_kill[i]);
			}

			fclose(f_sh);
    		
    		//char e[pid_length];

    		/*for(int bdx=0; bdx<6; bdx++){
    			for(int adx=0; adx<pid_length; adx++){
    				e[adx] = contenido[(bdx*pid_length)+adx];	
    			}

    			int ppid_new=atoi(e);
    			fprintf(f_sh,"%d", ppid_new);

    		}

			f_sh=fopen("PIDs_GETTY","w");
			printf("%s", contenido);
			fprintf(f_sh,"%s", contenido);
			fclose(f_sh);*/

		}
	} //falta eliminar el proceso si se ejecuta "shutdown" cuando se abra una nueva terminal
	
	printf("Fin");
	return 0;	
}

void sig_handler(int sig_kill)
{   
	for(int i=0;i<6;i++)
	{
		kill(arraypid[i],9);
	}
	
	kill(0,9);
}
