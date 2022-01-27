#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <math.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define TAMBUFFER 10

#define VELPROD 50000	// Microsegundos
#define VELCONS 10000

//#define LIMITE 1000

#define PRODUCTORES 4
#define FIN -1

struct STRBUFF {
	int ent;	// Donde va a estar el siguiente elemento que voy a meter al buffer
	int sal;	// Donde está el siguiente elemento que voy a sacar del buffer
	int buffer[TAMBUFFER];	// Buffer circular
};

struct TREE {
	int dato;
	struct TREE *left;
	struct TREE *right;
};
//ESTRUCTURA DEL BUZON
struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    unsigned int num;   /* message data */
};

//ENCABEZADOS DE LAS FUNCIONES A USAR
void tree_insert(struct TREE **root,int dato);
void tree_inorder(struct TREE *root);
void productor();
void consumidor();
int isprime(int n);

int inicio = 0;
int numeros_fin = 1000;
int queue;	// Buzón de mensajes

int main(int argc, char *args[])
{
    // Definición de variables
    int res;
    int n;
 	int p;
	int i;
	int shmid;
    int status;
    int rango = numeros_fin/PRODUCTORES;
	
    //PROPORCIONAR UN INICIO/FINAL POR EL USUARIO 
	if(argc > 2)
	    {
		inicio = atoi(args[1]);
		numeros_fin = atoi(args[2]);
		//rango = (numeros_fin-numeros_inicio)/PRODUCTORES;
		}
    else
        {
            
        }
	
    //	Crear Buzón
	 queue = msgget(0x1234, 0666|IPC_CREAT);
	 
     if(queue == -1)
	   {
		fprintf(stderr, "No se pudo crear el buzón :(\n");
		exit(1);
	   }
	
    /* Aquí se crean los procesos */
	for(i=0;i<PRODUCTORES;i++)
	{
		p=fork();
		if(p==0)
			productor(i);
	}

	p=fork();
	if(p==0)
		consumidor();


  
    for(n=0;n<PRODUCTORES;n++)
  		wait(&status);

    //ELIMINAMOS EL BUZON
    msgctl(queue, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);

}


void productor(int nprod)
{
    printf("Inicia productor\n");
    int n;
    struct msgbuf mensaje;
	int inicio = 1 + nprod*numeros_fin/PRODUCTORES;
	int limite = (nprod+1)*numeros_fin/PRODUCTORES;
    
    for(n=inicio;n<=limite;n++)
    {
		if(isprime(n) || n==limite)
		{
			
			if(n<limite)
			{
        		// Enviar n al consumidor
        		// Construye un mensaje
				mensaje.mtype = 1;	// Prioridad o tipo del mensaje
				mensaje.num = n;
				printf("Productor %d produce %d\n",nprod,mensaje.num);
                // Envíe el mensaje al buzón
				msgsnd(queue, &mensaje, sizeof(struct msgbuf), IPC_NOWAIT);	// No espera a que sea recibido
                //printf("men%d\n",mensaje.num);
                //sleep(1);

			}
			else
			{
				// Enviar 0 al consumidor
        		// Construye un mensaje
				mensaje.mtype = 1;	// Prioridad o tipo del mensaje
				mensaje.num = 0;
		
				// Envíe el mensaje al buzón
				msgsnd(queue, &mensaje, sizeof(struct msgbuf), IPC_NOWAIT);	// No espera a que sea recibido
                //sleep(1);
			}
 
        	usleep(rand()%VELPROD);
        	usleep(rand()%VELPROD);
		}
    }
    exit(0);
}


void consumidor()
{
	struct TREE *root=NULL;
    int productores=PRODUCTORES;
	int terminadores=0;
	int dato;
    struct msgbuf mensaje;
    int n=1;

    printf("Inicia Consumidor\n");
    //printf("dato%d\n",dato);
    while(productores)
    {
        msgrcv(queue, &mensaje, sizeof(struct msgbuf), 1, 0);
		//printf("mensaje:%d\n",mensaje.num);
        dato=mensaje.num;
		
        if(dato)
		{
            // En vez de mostrarlo, lo vamos a guardar en un contenedor de manera
			// que queden ordenados en orden ascendente, por ejemplo, un árbol binario
        	//printf("\t\t\tConsumidor consume %d\n",dato);
			tree_insert(&root,dato);
		}
		else
		{
			productores--;
			terminadores++;
			//printf("\t\t\tFINAL ENCONTRADO\n");
		}

        usleep(rand()%VELCONS);
        usleep(rand()%VELCONS);

    }
	
	// Mostrar todos los elementos que se guardaron de manera ordenada en el contenedor.
	tree_inorder(root);
	
	//printf("Terminadores %d\n",terminadores);
    //msgctl(queue, IPC_RMID, NULL);
    exit(0);
}
     
int isprime(int n)
{
	int d=3;
	int prime=0;
	int limit=sqrt(n);
	
	if(n<2)
		prime=0;
	else if(n==2)
		prime=1;
	else if(n%2==0)
		prime=0;
	else
	{
		while(d<=limit && n%d)
			d+=2;
		prime=d>limit;
	}
	return(prime);
}

//ARBOL BINARIO

void tree_insert(struct TREE **root,int dato)
{
	if(*root==NULL)
	{
		*root=malloc(sizeof(struct TREE));
		(*root)->dato=dato;
		(*root)->left=NULL;
		(*root)->right=NULL;
	}
	else if(dato < (*root)->dato)
		tree_insert(&(*root)->left,dato);
	else if(dato > (*root)->dato)
		tree_insert(&(*root)->right,dato);
	
}

void tree_inorder(struct TREE *root)
{
	if(root->left!=NULL)
		tree_inorder(root->left);
	printf("primo: %d\n",root->dato);
	if(root->right!=NULL)
		tree_inorder(root->right);
	
}
