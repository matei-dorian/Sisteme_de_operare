# include<stdio.h>
# include<stdlib.h>
# include<sys/types.h>
# include<unistd.h>
# include<errno.h>
# include<sys/wait.h>

int main(int argc, char* argv[])
{

	printf("Starting parent %d\n", getpid());
	
	for(int i = 1; i <= argc; i ++)
	{	
		pid_t pid = fork();
		
		if(pid < 0)
			return(errno);
		else if(pid == 0)
		{	
			char* child_argv[] = {"./collatz", argv[i], NULL};
			execve("/home/matei/Desktop/lab_so/collatz",child_argv, NULL);
			perror(NULL); 
			printf("%s\n", argv[i]);
			
		}
		else wait(NULL);
	}

	printf("Done! Parent %d Me %d\n", getppid(), getpid());
	return 0;
}


