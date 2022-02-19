# include<stdio.h>
# include<stdlib.h>
# include<sys/types.h>
# include<unistd.h>
# include<errno.h>
# include<sys/wait.h>

int main(int argc, char* argv[])
{

	pid_t pid = fork();
		
	if (pid < 0)
		return errno;
	
	else if (pid == 0)
	{
		int x = atoi(argv[1]);
		printf("%d: ", x);

		while(x != 2)
		{	if(x % 2)
				x = 3 * x + 1;	
			else
				x = x / 2;
			printf("%d ", x);		
		}

		printf("1.\n");
	
	}

	else
	{
		wait(NULL);
		printf("Done! Parent %d Me %d\n", getppid(), pid);
	}		

	return 0;
}
