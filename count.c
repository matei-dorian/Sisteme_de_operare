#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>

#define MAX_RESOURCES  10
#define MAX_THREADS    12


int available_resources = MAX_RESOURCES;
pthread_mutex_t mtx;

int decrease_count ( int count )
{
	pthread_mutex_lock(&mtx);                /// allow only one thread at a time to make changes
	if ( available_resources < count ){
		pthread_mutex_unlock(&mtx);      /// let another thread try to use remaining resources
		return -1;
	}
	else{	

		available_resources -= count;
		pthread_mutex_unlock(&mtx);     /// allow other threads to use resources
		
	}
	return 0;
}


int increase_count ( int count )
{	
	pthread_mutex_lock(&mtx);               /// allow only one thread at a time to make changes
	available_resources += count;
	pthread_mutex_unlock(&mtx);             /// let other threads give back resources
	return 0;
}


void* count(void *arg)
{
	int *id = (int*) arg;
	int nr = rand() % MAX_RESOURCES + 1;
	 
	while(decrease_count(nr) == -1) continue;

	printf("Thread %d got %d resources %d remaining\n", *id, nr, available_resources);

	increase_count(nr);
	return NULL;
}


int main(int argc, char* argv[])
{
	pthread_t thr[MAX_THREADS];
	int i;

	/// initialize the mutex
	if (pthread_mutex_init(&mtx, NULL)){
		perror ("Error at initializing the first mutex");
		return errno ;
	}

	
	srand(time(0));     /// for generating a diffrent random sequel of numbers on every call

	/// create the threads
	for(i = 0; i < MAX_THREADS; i++)
	{	
		int *th_id = (int*)malloc(1 * sizeof(int)); 
		*th_id = i;
		if (pthread_create(&thr[i], NULL, count, (void*)th_id)){
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


	/// free the mutex
	pthread_mutex_destroy (&mtx);
	return 0;
}
