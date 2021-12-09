#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[])
{
    
    FILE *f_sh;

    f_sh=fopen("PIDs_GETTY","r");
    char array[5];
    char contenido[100];
    int contenido_i[6];
    int longitud;

    array[5]=0;     // Es necesario para que el atoi identifique el fin de la cadena

    longitud=fread(contenido,1,24,f_sh);
    
    fclose(f_sh);

    for(int i=0;i<6;i++){
        for(int i2=0;i2<4;i2++){
            array[i2] = contenido[(i*4)+i2];
            //printf("\n%c\n", array[i2]);
        }
        contenido_i[i]=atoi(array);

        printf("\n%d\n", contenido_i[i]);
    }


}