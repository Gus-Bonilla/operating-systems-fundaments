#include <scheduler.h>
#include <stdio.h>

#define PRIORITY_LEVELS 20

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;
QUEUE ready[20];	// Arreglo de colas de listos en el que cada elemento representa un nivel de prioridad
QUEUE waitinginevent[MAXTHREAD];
int currpriority = 0;	// Prioridad actual del proceso en ejecución, 0->PrioridadMaxima, 19->PrioridadMinima

void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;
	
	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos
		threads[callingthread].status=READY;
		_enqueue(&ready[0],callingthread);	// Se agraga hilo a la cola de listos de prioridad máxima
	}
	
	if(event==BLOCKTHREAD)
	{
		threads[callingthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],callingthread);
		changethread=1;
	}

	if(event==ENDTHREAD)
	{
		threads[callingthread].status=END;
		changethread=1;
	}
	
	if(event==UNBLOCKTHREAD)
	{
			threads[callingthread].status=READY;
			_enqueue(&ready[0],callingthread);	// Se agraga hilo a la cola de listos de prioridad máxima
	}

	if(event==TIMER)
	{
		// Agragando el thread en ejecucion a cola de listos
		int flag_more_threads_ready = 0;	// Flag para evaluar si hay al menos un hilo en una de las colas de listos

		for(int i=0; i<PRIORITY_LEVELS; i++)
		{
			// Buscando hilos en colas de listos 
			if(!_emptyq(&ready[i]))
	       	{
		   		flag_more_threads_ready = 1;
		   	}
		}

		threads[currthread].status=READY;	// Se cambia el estado del hilo a READY
		
		if(currpriority<19)	// El hilo tiene nivel de prioridad menor a 19?
	    { 
			if (flag_more_threads_ready==0)	// No hay más de un hilo en las colas de listos?
	        {  
	            _enqueue(&ready[currpriority], currthread);	// Se agrega hilo a cola de listos con el mismo nivel de prioridad
	        }
            else
           	{    
				_enqueue(&ready[currpriority+1], currthread);	// Se agrega hilo a cola de listos con el nivel de prioridad que tenía más uno
		   	}	  
	    }
	    else 
       	{
			_enqueue(&ready[currpriority], currthread);	// Se agrega hilo a cola de listos con el mismo nivel de prioridad
	   	}
		
		changethread=1;	// Hay que hacer cambio de contexto
	}

	if(changethread)	// Hay que hacer cambio de contexto?
	{
		old=currthread;	// El hilo en ejecución será desplazado

		for (int i=0; i<PRIORITY_LEVELS; i++)
		{
			// Busqueda de otros hilos en las colas de listos 
		  	if(!_emptyq(&ready[i]))           // Hay otros procesos en la cola de listos de prioridad i?
		    {  
				next = _dequeue(&ready[i]);   // El siguiente hilo en ejecucion será el obtenido de la cola de listos 
				currpriority = i;             // La prioridad del hilo es igual a la iteracion i (Nivel de prioridad en cola de listos)
				break;	// Salir del ciclo                          
			}
		}
		
		threads[next].status = RUNNING;	// El estado del siguiente hilo en ejecucion se pasa a RUNNING 
		_swapthreads(old, next);	// Cambio de hilo en ejecución
	}
}
