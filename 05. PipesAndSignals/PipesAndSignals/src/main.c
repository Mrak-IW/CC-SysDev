#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "processFunctions.h"

void sighandler(int signum)
{
	printf("Parent: SIG:%d received\n", signum);
}

int main(void)
{

	pid_t child_pid;
	pid_t parent_pid = getpid();
	int pipeEnds[2];
	pipe(pipeEnds);

	child_pid = fork();
	if(0 != child_pid)
	{
		//This is a parent process

		signal(SIGUSR1, &sighandler);
		firstProcessFunction(pipeEnds);

		printf("Parent: Waiting for a signal USR1\n");

		/* Дожидаемся завершения дочернего процесса. */
		waitpid(child_pid, NULL, 0);
		printf("Parent: Child process is over\n");
	}
	else
	{
		secondProcessFunction(pipeEnds);
		printf("Child: I'll kill you, parent!!!\n");
		kill(parent_pid, SIGUSR1);
	}

	return 0;
}
