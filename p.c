#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>

#define MAX_SUBJECTS 6

int main (int argc, char *argv[]) 
{
	int pipeInfd[2];	// File descriptor of pipe
	int pipeOutfd[2]; // File descriptor of Outgoing pipe
	pid_t pid;	// Process ID
	int mark[MAX_SUBJECTS];
	
	if (pipe(pipeInfd) == -1)
	{
		printf("Pipe Failed\n");
		exit(EXIT_FAILURE);
	}

	if (pipe(pipeOutfd) == -1)
	{
		printf("Pipe Failed\n");
		exit(EXIT_FAILURE);
	}
	
	// Input the details
	printf("Enter the 6 marks of the student: ");
	for (int i = 0; i < MAX_SUBJECTS; i++)
	{
		scanf("%d", &mark[i]);
	}

	pid = fork();
	//printf("Hi I am %d\n", (int)getpid());
	
	if (pid < 0) 
	{
		printf("Fork failed\n");
		exit(EXIT_FAILURE);
	}
	
	else if (pid > 0)
	{
		// Parent process block
		printf("Sending student marks to child process...\n");
		close(pipeInfd[0]);	// Closes the unused read end of the pipe
		close(pipeOutfd[1]);
		write(pipeInfd[1], mark, sizeof(mark)/sizeof(int));
		close(pipeInfd[1]);
			//printf("Parent waiting for child process to complete\n");
		wait(NULL);
		char *R;
		read(pipeOutfd[0], &R, 4);
		close(pipeOutfd[0]);
		printf("Result: %s\n", R);
		printf("Parent process terminating\n");
		
	}
	
	else 
	{
		// Child process block	
		printf("Child process recieving marks...\n");
		close(pipeInfd[1]);	// Closes the unused write end of the pipe
		close(pipeOutfd[0]);// Closes the unused write end of the pipe
		int temp;
		int sum = 0;
		for (int j = 0; j <= MAX_SUBJECTS; j++) {
			read(pipeInfd[0], &temp, sizeof(int));
			sum = sum + temp;
		}
		close(pipeInfd[0]);
		sum = sum/6;
		
		char *r;
		
		if (sum >= 40)
			r = "Pass";
		else
			r = "Fail";
			
		write(pipeOutfd[1], &r, 4);
		close(pipeOutfd[1]);
		
		exit(0);
	}	
}
