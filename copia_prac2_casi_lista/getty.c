#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define TAM_CONT 1500


int main()
{
    char contra[6][16];
    char usuarios[6][16]={};
    char usr[10];
    char clave[10];
    char shell[10];
    int  longitud;
    int  flag_usuario=1;
    FILE *pass;
    FILE *f_sh;
    
    pass=fopen("passwd","r");
    char contenido[TAM_CONT];
    longitud=fread(contenido,1,TAM_CONT,pass);

    fclose(pass);

    f_sh=fopen("PIDs_GETTY","a");
    fprintf(f_sh,"%d",getppid());
    fclose(f_sh);
    
    printf("Bienvenido\n");
    int i=0;
    int k=0;
    
    for(int j=0;j<longitud;j++)
    {  
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
    for(int idx=0;idx<6;idx++)
    {
    //printf("%s\n",usuarios[idx]);

    if(strcmp(usr,usuarios[idx])==0)
      {
          valid_user=1;
          if(strcmp(clave,contra[idx])==0)
          {
          printf("Acesso correcto\n");
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
    printf("Sesión terminada\n");
    }
    return 0;
      
}

