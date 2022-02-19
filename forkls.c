# include<stdio.h>
# include<sys/types.h>
# include<unistd.h>
# include<errno.h>
# include<sys/wait.h>

int main(int argc, char* argv[]){
	pid_t pid = fork();
	
	if(pid < 0)
		return errno;
	else if (pid == 0)
	{ 
		char *execve_argv[] = {"ls", NULL};
		execve("/bin/ls", execve_argv, NULL);
		printf("Dupa execve\n");
	}
	else
	{	
		printf("My PID = %d, Child PID = %d\n", getpid(), pid);	
		wait(NULL);
		printf("Child %d finished\n", pid);
	}

	return 0;
}
