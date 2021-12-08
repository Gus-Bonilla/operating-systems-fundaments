#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

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
		
		if(pid == 0){
			printf("PID del proceso%d,%d\n",i,getpid());
			execlp("xterm","xterm","-e","./getty",NULL);
		}
		
	}

	wait(NULL);
	/*while(1)
	{
		wait(NULL);
		pid=fork();
		//if(pid==0)
			//execlp("xterm","xterm","-e","./getty",NULL);
	}*/
	
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
