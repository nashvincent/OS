#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>

#define MAX_SUBJECTS 6

int main (int argc, char *argv[]) 
{
	int pipefd[2];	// File descriptor of pipe
	int pipeOutfd[2]; // File descriptor of Outgoing pipe
	pid_t pid;	// Process ID
	int mark[MAX_SUBJECTS];
	
	if (pipe(pipefd) == -1)
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
	// TODO
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
		// TODO
		printf("Sending student marks to child process...\n");
		close(pipefd[0]);	// Closes the unused read end of the pipe
		write(pipefd[1], mark, sizeof(mark)/sizeof(int));
		close(pipefd[1]);
			//printf("Parent waiting for child process to complete\n");
		wait(NULL);
		printf ("Parent process terminating\n");
		
		/*
		char result;
		read(pipefd[0], &result, 1);
		if (result == 'P')
			printf("PASS\n");
		else
			printf("FAIL\n");
			
		close(pipefd[0]); */		
	}
	
	else 
	{
		// Child process block
		// TODO	
		printf("Child process recieving marks...\n");
		close(pipefd[1]);	// Closes the unused write end of the pipe
		int temp;
		int sum = 0;
		for (int j = 0; j <= MAX_SUBJECTS; j++) {
			read(pipefd[0], &temp, sizeof(int));
			sum = sum + temp;
		}
		close(pipefd[0]);
		sum = sum/6;

		if (sum >= 40)
			printf("Result: Pass\n");
		else
			printf("Result: Fail\n");
		
		exit(0);
	}
	
}