#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define TAM_CONT 1500


int main()
{
    //char usuario[TAM];
    char contra[6][16];
    char usuarios[6][16]={};
    char usr[10];
    char clave[10];
    char shell[10];
    int  longitud;
    int  flag_usuario=1;
    FILE *pass;
    //int k=0;
    pass=fopen("passwd","r");
    char contenido[TAM_CONT];
    longitud=fread(contenido,1,TAM_CONT,pass);

    fclose(pass);
    
    printf("contenido:");
    int i=0;
    int k=0;
    
    for(int j=0;j<longitud;j++)
    {  
        printf("%c",contenido[j]);
        //usuarios[i][k]=contenido[j];
        if('\n'==contenido[j])
        {
          contra[i][k-1]=0;
          i++;  
          flag_usuario=1;
          k=0;
        }
        else 
        {
          if(':'==contenido[j])
          {
              flag_usuario=0;
              usuarios[i][k]=0;
              k=0; 
          }
          else
          {
            if(flag_usuario)
              usuarios[i][k]=contenido[j];
              else
                contra[i][k-1]=contenido[j];

          }
          k++;

        }
        
        
    }
    while(1){
    printf("INGRESE USUARIO:\n");
    scanf("%s",usr);
    printf("INGRESE CONTRASEÑA\n");
    scanf("%s",clave);
    int valid_user=0;
    int valid_contra=0;
    //if(strcmp(usr,"shutdown"))
          
      //   break;
       
    for(int idx=0;idx<6;idx++)
    {
    printf("%s\n",usuarios[idx]);

    if(strcmp(usr,usuarios[idx])==0)
      {
          valid_user=1;
          if(strcmp(clave,contra[idx])==0)
          {
          printf("si son\n");
          valid_contra=1;
          break;
          }
      }
    }
    if ((valid_user==0 || valid_contra==0))
      {
       printf("Usuario y contraseña incorrectos\n");
       continue;
      }

    int pid;
    pid=fork();
    if (pid==0)
    {
      execlp("./sh","sh",NULL);
    }
    wait(NULL);
    /*int reg;
    int sigseg_t=set;
    int sig
    //reg=waitid(P_PID,pid,NULL,WEXITED);
    
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    sigprocmask( SIG_BLOCK, &set, NULL );
    printf("Waiting for a SIGINT signal\n");
    reg = sigwait(&set,sigptr);
    printf("SOY REG %d",reg);*/
    //if(reg!=0)
    //{
    //kill(getppid(),9);
    //}
    }
    //printf("holis\n");
    //printf("%d\n",flag_usuario);
    return 0;
      

}

