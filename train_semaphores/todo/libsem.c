#include <pthread_utils.h>
#include <libsem.h>

// Tren => Proceso/Hilo


// Macro que incluye el código de la instrucción máquina xchg
#define atomic_xchg(A,B) 	__asm__ __volatile__(	\
							"   lock xchg %1,%0 ;\n"	\
							: "=ir"  (A)			\
							: "m"  (B), "ir" (A)		\
							);


int g=0;	// Semáforo para hacer uso de las funciones waitsem y signalsem


// Inicialización de el semaforo para exclución mutua de los trenes
void initsem(SEMAPHORE *s, int val)
{
	s->count = val;			// Se inicializa con 1 para que solo un tren pueda pasar a la vez
	initqueue(&(s->queue));	// Cola de espera para ejecutar sección crítica
}


// Función wait que manda a cola de espera y bloquea el tren si la sección crítica no está disponible
void waitsem(SEMAPHORE *s)
{
	int l = 1;

	do { atomic_xchg(l, g); } while(l!=0);		// Espera a que esté disponible el uso de las funciones waitsem y signalsem

	//////////########## INICIA SECCIÓN CRÍTICA ##########//////////
	
	s->count -= 1;								// Disminuye la cantidad de procesos que pueden hacer uso de la sección crítica

	if(s->count < 0)							// Si count es igual o mayor a 0 se continúa con el funcionamiento del tren; si no, se pasa a cola de espera
	{
		enqueue(&(s->queue), pthread_self());	// Tren introducido en cola de espera
	
	/////////########## TERMINA SECCIÓN CRÍTICA ##########//////////

		g = 0;									// Liberacíon del uso de las funciones signalsem y waitsem
		block_thread();							// Bloqueo del tren
	}
	else g = 0;									// Liberacíon del uso de las funciones signalsem y waitsem
}


// Función signal que saca de la cola de espera y desbloquea el siguiente tren en la cola si la sección crítica está disponible
void signalsem(SEMAPHORE *s)
{
	int l = 1;

	do { atomic_xchg(l, g); } while(l!=0);		// Espera a que esté disponible el uso de las funciones waitsem y signalsem

	//////////########## INICIA SECCIÓN CRÍTICA ##########//////////
	
	s->count += 1;								// Aumenta la cantidad de procesos que pueden hacer uso de la sección crítica

	if(s->count <= 0)							// Si count es mayor a 0 se mantiene el estado actual de los trenes; si no, se saca de la cola de espera el siguiente tren en la cola
	{
		pthread_t ret = dequeue(&(s->queue));	// Tren extraido de cola de espera
	
	/////////########## TERMINA SECCIÓN CRÍTICA ##########//////////

		g = 0;									// Liberacíon del uso de las funciones signalsem y waitsem
		unblock_thread(ret);					// Desbloqueo del tren
	}
	else g = 0;									// Liberacíon del uso de las funciones signalsem y waitsem
}
