#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#define CORES_NUMBER 2

void calc_pi_4(unsigned long int *values);
long double calc_leibinz(unsigned long int iteration);

long double partial_results[CORES_NUMBER];

int main(int argc, char *args[]){
	unsigned long int iterations;
	long double leibinz_result = 0;
	
	unsigned long long start_tu;
	unsigned long long stop_tu;
	unsigned long long elapsed_time_u;
	unsigned long long start_ts;
	unsigned long long stop_ts;
	long long elapsed_time_s;
	struct timeval ts;

	pthread_t tid[CORES_NUMBER];
    unsigned long int args_1[CORES_NUMBER][2];

	if(argc < 2)
		iterations = 2000000000;
		//iterations = 100;
	else
		iterations = atoi(args[1]);

	printf("\nIteraciones a realizar: %li\n\n", iterations);

	gettimeofday(&ts, NULL);
	start_tu = ts.tv_usec; // Tiempo inicial
	start_ts = ts.tv_sec; // Tiempo inicial

	for(int idx=0; idx<CORES_NUMBER; idx++){
        args_1[idx][0] = idx;
        args_1[idx][1] = iterations;
        args_1[idx][2] = CORES_NUMBER;

        pthread_create(&tid[idx], NULL, calc_pi_4, &args_1[idx][0]);
    }

    for(int idx=0; idx<CORES_NUMBER; idx++)
        pthread_join(tid[idx], NULL);

    for(int idx=0; idx<CORES_NUMBER; idx++)
    	leibinz_result += partial_results[idx];

	gettimeofday(&ts, NULL);
	stop_tu = ts.tv_usec; // Tiempo final
	stop_ts = ts.tv_sec; // Tiempo final
	elapsed_time_s = stop_ts - start_ts;
	seg != useg //inicio 2seg +  5useg = tiempo total
	final//12seg +  1000001useg = tiempo total
	if()
	elapsed_time_u = stop_tu - start_tu;

	printf("\nEl resultado de la serie de Leibinz fue de: %.20LF\n\n", leibinz_result);
	printf("\nPi calculado con la serie de Leibinz es de: %.20LF\n\n", leibinz_result*4);
	printf("---------------------------------------------\n");
	printf("TIEMPO TOTAL, %lld micro segundos\n",elapsed_time_u);
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time_s);

	return 0;
}


void calc_pi_4(unsigned long int *values){
	int iter_start, iter_jump, mod;
	unsigned long int thread_id = *values;
	unsigned long int iter_number = *(values+1);
	unsigned long int cores = *(values+2);
	long double leibinz_result = 0;

	iter_jump = iter_number / cores;
	iter_start = iter_jump * thread_id;
	mod = iter_number%cores;

	if(thread_id+1 == cores)
		iter_jump += mod;

	for(int idx=iter_start; idx<(iter_start+iter_jump); idx++){
		if(idx%2==0 || idx==0)
			leibinz_result += calc_leibinz(idx);
		else
			leibinz_result -= calc_leibinz(idx);
	}

	partial_results[thread_id] = leibinz_result;
}

long double calc_leibinz(unsigned long int iteration){
	return (long double)1/((iteration*2)+1);
}