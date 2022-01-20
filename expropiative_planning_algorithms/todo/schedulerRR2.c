#include <scheduler.h>

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;
int quantum = 0;           // Se inicializa el valor de quantum, variable para identificar el quantum actual

QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];

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
		_enqueue(&ready,callingthread);
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
			_enqueue(&ready,callingthread);
	}

	if(event==TIMER && quantum>=1)        
	{
		//pasando el thread en ejecucion a listo
		threads[currthread].status=READY;    //se verifica el estado del hilo
		_enqueue(&ready,currthread);         //se pone en cola el hilo 
		changethread=1;                      //se cambia de hilo
		quantum=0;                           //se reinicia el valor de quantum
	}

	else
	{   
		//se incrementa el valor de quantum en este caso el limite es 1 por lo que quantum=1
	    quantum++;                       	
	}

	if(changethread)
	{
		old=currthread;
		next=_dequeue(&ready);
		
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}

}
