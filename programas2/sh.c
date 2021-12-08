#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
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
			//kill(-1,getpid());
            //exit(EXIT_FAILURE);
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
