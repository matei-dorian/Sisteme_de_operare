#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>


#define MAX_THREADS  10
#define N            5


int counter_in, counter_out;
pthread_mutex_t mtx;
sem_t sem;

int init_barrier(int n){

	/// initialize the mutex
	if (pthread_mutex_init(&mtx, NULL)){
		perror ("Error at initializing the mutex");
		return errno ;
	}

	/// initialize the semaphore
	if ( sem_init (& sem , 0 , n)) {
		perror ("Error at initializing the semaphore");
		return errno ;
	}

	/// initialize the counters
	counter_in = 0;
	counter_out = 0;

	printf("NTHRS = %d\n", n);
	return 0;
}


void* barrier(void *arg)
{
	int *id = (int*) arg;
	
	sem_wait(&sem);                              /// if we are still waiting for threads decrease the value in sem
	
	pthread_mutex_lock(&mtx);                    /// count the threads that are inside the barrier
	counter_in++; 
	printf ("%d reached the barrier\n" , *id);
	pthread_mutex_unlock(&mtx);
	
	

	while(counter_in < N) continue;              /// when we get N threads release all of them
	
	
	pthread_mutex_lock(&mtx);                    /// count the threads released from the barrier
	printf ("%d passed the barrier\n" , *id);
	counter_out++;
	pthread_mutex_unlock(&mtx);
		
	pthread_mutex_lock(&mtx);
	if(counter_out == N){                        /// the last thread to exit the barrier will reset it
				
		for(int i = 1; i <= counter_out; i++) 
			sem_post(&sem); 
		counter_in = 0;
		counter_out = 0;
		printf("\n");
		
	}
	pthread_mutex_unlock(&mtx);
	
	
	return NULL;
}


void destroy_barrier(){
	/// free the mutex and the semaphore
	pthread_mutex_destroy(&mtx);
	sem_destroy(&sem );
}


int main(int argc, char* argv[])
{
	pthread_t thr[MAX_THREADS];
	int i;

	init_barrier(N);

	/// create the threads
	for(i = 0; i < MAX_THREADS; i++)
	{	
		int *th_id = (int*)malloc(1 * sizeof(int)); 
		*th_id = i;
		if (pthread_create(&thr[i], NULL, barrier, (void*)th_id)){
			perror("Error at thread creation");
			return errno;
		}
	}


	/// join the threads
	for(i = 0; i < MAX_THREADS; i++)
	{	
		if (pthread_join(thr[i], NULL))
		{
			perror("Error at join");
			return errno;
		}
	}

	destroy_barrier();
	
	return 0;
}
