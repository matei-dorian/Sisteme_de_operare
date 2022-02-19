#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>
#include<errno.h>

int main(int argc, char* argv[]){

	char shm_name[] = "myshm";
	int shm_fd;
	int shm_size = 10 * 4096; /// vom limita functia la maxim 10 argumente si 1048 de numere la fiecare argument

	printf("Starting parent %d\n", getpid());

	if(argc > 11)
	{
	  	printf("Too many arguments!\n");
	  	return 0;
	}

	shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, \
			   S_IRUSR | S_IWUSR);

	if(shm_fd < 0){
		perror("Error at shm_open\n");
		return errno;
	}

	if(ftruncate(shm_fd, shm_size) == -1){
		perror("Error at truncate\n");
		shm_unlink(shm_name);
		return errno;
	}


	for(int i = 1; i < argc; i++)
	{
		pid_t pid = fork();

		if(pid < 0)
			return errno;
		else if(pid == 0)
		{
			int *v = (int*)mmap(0, shm_size/10, PROT_WRITE, \
			                    MAP_SHARED, shm_fd, (i - 1)*4096); /// ar trebui (i-1) * shm_size / 10
			int x = atoi(argv[i]);
			int j = 1;

			*v = x;

			while(x != 1 && j < 1048)
			{

				if(x % 2)
				  x = 3 * x + 1;
				else
				  x = x / 2;

				*(v + j) = x;
				j ++;

			}

			printf("Done Parent %d Me %d\n", getppid(), getpid());
			munmap((void*)v, shm_size/10);
			return 0;

		}
		else
			wait(NULL);

	}


	for(int i = 1; i < argc; i++)
	{
		int *v_p = (int*)mmap(0, shm_size/10, PROT_READ, \
				     MAP_SHARED, shm_fd, (i-1)*4096); /// ar trebui (i-1) * shm_size / 10
		printf("%s: ", argv[i]);

		int j = 0;
		do
		{
			printf("%d ", v_p[j]);
			j++;
		}
		while(v_p[j - 1] != 1 || j <= 1048);

		printf("\n");
		munmap((void*)v_p, shm_size/10);

	}

	shm_unlink(shm_name);

	printf("Done Parent %d Me %d\n", getppid(), getpid());



	return 0;
}

