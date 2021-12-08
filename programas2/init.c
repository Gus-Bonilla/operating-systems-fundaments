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
	
	// Poner mi PID en un archivo
	f_sh=fopen("PID_INIT","w");
	fprintf(f_sh,"%d",getpid());
	fclose(f_sh);
    
    printf("PID_INIT (PROCESO PADRE): %d\n",getpid());
	
	for(i=0;i<6;i++)
	{
		pid = fork();
		
		if(pid == 0)
		{
			printf("PID del proceso%d,%d\n",i,getpid());
			execlp("xterm","xterm","-e","./getty",NULL);
		}
		
	}

	//wait(NULL);
	while(1)
	{
		int killed_term = wait(NULL);
        int killed_start=0;

        f_sh=fopen("PIDs_GETTY","r");
		char contenido[TAM_CONT];
		char array[pid_length];
		int longitud;
		longitud=fread(contenido,1,TAM_CONT,f_sh);
        fclose(f_sh);

        for(int i=0;i<6;i++)
	   { 
         for(int idx=0;idx<pid_length;idx++)
		 {
			 array[idx]=contenido[(i*pid_length)+idx];
		 }
		 int pid_kill=atoi(array);
		 	if(pid_kill==killed_term){
		 		killed_start = i * pid_length;
		 		break;
		 	}
		 	
	   }

		pid=fork();

		if(pid==0)
			execlp("xterm","xterm","-e","./getty",NULL);
		else{
			char k[pid_length];
    		sprintf(k, "%i", pid);

    		for(int adx=0; adx<pid_length; adx++){
    			contenido[adx+killed_start] = k[adx];
    		}
			

			int ppid_kill=atoi(array_ppid);
			// LAMMAR A FUNCION DE BORRADO
    		f_sh=fopen("PIDs_GETTY","a");
    		char e[pid_length];
    		
    		for(int bdx=0; bdx<6; bdx++){
    			for(int adx=0; adx<pid_length; adx++){
    				e[adx] = contenido[(bdx*pid_length)+adx];	
    			}

    			int ppid_new=atoi(e);
    			fprintf(f_sh,"%d", e);

    		}

    		
    		fclose(f_sh);


			f_sh=fopen("PIDs_GETTY","w");
			printf("%s", contenido);
			fprintf(f_sh,"%s", contenido);
			fclose(f_sh);

		}
	} //falta eliminar el proceso si se ejecuta "shutdown" cuando se abra una nueva terminal
	
	printf("Fin");
	return 0;	
}

void sig_handler(int sig_kill)
{   for(int i=0;i<6;i++)
	{
		kill(arraypid[i],9);
	}
	kill(0,9);
}
