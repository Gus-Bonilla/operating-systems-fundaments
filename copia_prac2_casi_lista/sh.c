#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#define pid_length 4  //consideramos que cada pid tiene un tamaño de 4
#define TAM_CONT 100
void getstr(char *str)
{
	fgets(str,80,stdin);   
	while(*str!='\n' && *str!='\n')
		str++;
	*str='\0';
}


int main()
{   int pid;
	char cmd[80];
	int continua=1;
	int longitud,lo;
	char array[pid_length];
	char array_ppid[pid_length];
	FILE *pass;
	FILE *p;
	while(continua)
	{   
		//printf("%d\n",getppid());
		printf("sh>");
		getstr(cmd);
		if(strcmp(cmd,"exit")==0)
		    {
			continua=0;
			}
		else if(strcmp(cmd,"shutdown")==0)
		    {
			printf("asesino\n");
            pass=fopen("PIDs_GETTY","r");
    		char contenido[TAM_CONT];
    		longitud=fread(contenido,1,TAM_CONT,pass);
            fclose(pass);
			if(longitud>=6*pid_length)
	           {
				   
				   for(int i=0;i<6;i++)
				   { 
                     for(int idx=0;idx<pid_length;idx++)
					 {
						 array[idx]=contenido[(i*pid_length)+idx];
					 }
					 int pid_kill=atoi(array);
					 	if(pid_kill!=getppid())
					 	kill(pid_kill,9);
				   }
			   }
            p=fopen("PID_INIT","r");
    		char cont[TAM_CONT];
    		lo=fread(cont,1,TAM_CONT,p);
            fclose(p);
			for(int i=0;i<lo;i++)
				{
				  array_ppid[i]=cont[i];
				}
				int ppid_kill=atoi(array_ppid);
				kill(ppid_kill,9);
				kill(getppid(),9);
			continua=0;
			}
		
		else
		{
			pid=fork();
			if(pid==0)
			{
            execlp(cmd,cmd,NULL);
			}
		wait(NULL);	
		
		}
			//system(cmd); // No lo podemos usar
						 // Hay que hacerlo con las llamadas fork, exec y wait 
	}
}
