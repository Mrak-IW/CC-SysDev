#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
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
	char *fifoName = "./fifo0001.1";

	if(mkfifo(fifoName, S_IRWXO | S_IRWXG | S_IRWXU))
	{
		fprintf(stderr, "Can't create fifo\n");
		struct stat filestatus;
		if(stat(fifoName, &filestatus))
		{
			exit(0);
		}
		else
		{
			fprintf(stderr, "Reason: fifo has already been created\n");
		}
	}

	child_pid = fork();
	if(0 != child_pid)
	{
		//This is a parent process

		signal(SIGUSR1, &sighandler);
		firstProcessFunction(pipeEnds);

		printf("Parent: Waiting for a signal USR1\n");

		int fd = open(fifoName, S_IRGRP | S_IRGRP | S_IROTH);
		//dup2(STDOUT_FILENO, fd);
		dup2(fd, STDIN_FILENO);

		char buf[255];
		memset(buf, 0, 255);

		//read(STDIN_FILENO, buf, 254);
		fgets(buf, 254, stdin);
		printf("Parent: Received: %s\n", buf);

		//read(STDIN_FILENO, buf, 254);
		fgets(buf, 254, stdin);
		printf("Parent: Received: %s\n", buf);

		/* Дожидаемся завершения дочернего процесса. */
		waitpid(child_pid, NULL, 0);
		printf("Parent: Child process is over\n");
		close(fd);
	}
	else
	{
		secondProcessFunction(pipeEnds);
		char msg[] = "Communication via named pipe\n";
		int fd = open(fifoName, S_IWGRP | S_IWUSR | S_IWOTH);
		write(fd, msg , 29);
		close(fd);

		FILE* npipe = fopen(fifoName, "w");
		fprintf(npipe, "Communication via named pipe 2\n");
		fclose(npipe);

		printf("Child: I'll kill you, parent!!!\n");
		kill(parent_pid, SIGUSR1);
	}

	return 0;
}
