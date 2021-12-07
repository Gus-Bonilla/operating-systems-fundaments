#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define CORES_NUMBER 2

long double calc_leibinz(unsigned long int iteration);

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

	if(argc < 2)
		iterations = 2000000000;
		//iterations = 100;
	else
		iterations = atoi(args[1]);

	printf("\nIteraciones a realizar: %li\n\n", iterations);

	gettimeofday(&ts, NULL);
	start_tu = ts.tv_usec; // Tiempo inicial
	start_ts = ts.tv_sec; // Tiempo inicial

	for(int idx=0; idx<iterations; idx++){
		if(idx%2==0 || idx==0)
			leibinz_result += calc_leibinz(idx);
		else
			leibinz_result -= calc_leibinz(idx);
	}

	gettimeofday(&ts, NULL);
	stop_tu = ts.tv_usec; // Tiempo final
	stop_ts = ts.tv_sec; // Tiempo final
	elapsed_time_s = stop_ts - start_ts;
	elapsed_time_u = stop_tu - start_tu;

	

	printf("\nEl resultado de la serie de Leibinz fue de: %.20LF\n\n", leibinz_result);
	printf("\nPi calculado con la serie de Leibinz es de: %.20LF\n\n", leibinz_result*4);
	printf("------------------------------\n");
	printf("TIEMPO TOTAL, %lld micro segundos\n",elapsed_time_u);
	printf("TIEMPO TOTAL, %lld segundos\n",elapsed_time_s);

	return 0;
}

long double calc_leibinz(unsigned long int iteration){
	return (long double)1/((iteration*2)+1);
}