#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>


void* reverse(void *arg){
	
	char* str = (char*) arg;
	int lg = strlen(str);
	char* res = (char*) malloc(lg * sizeof(char));

	for(int i = 0; i < lg; i++)
		res[lg - i - 1] = str[i];
	res[lg] = 0;
 
	return res;
}

int main(int argc, char* argv[]){
	
	/// test if the function was called corectly
	if(argc < 2){
		printf("No string to be reversed\n");
		return 1;
	}

	if(argc > 2){
		printf("Too many arguments\n");
		return 2;
	}

	/// start a new thread
	pthread_t thr;

	if(pthread_create(&thr, NULL, reverse, argv[1])){
		perror("Error at create");
		return errno;
	}

	
	/// wait for the result
	void *result;
	
	if(pthread_join(thr, &result)){
		perror("Error at join");
		return errno;	
	}
	
	/// print the result and free the memory
	printf("%s\n", (char*)result);
	free(result);

	return 0;
}


