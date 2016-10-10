#include <stdio.h>
#include <signal.h>
//#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void parentSigHandler(int signum, siginfo_t *siginfo, void * context)
{
	union sigval value = siginfo->si_value;

	printf("Parent: SIG:%d received\n", signum);
	printf("Parent: VAL:%d received\n", value.sival_int);
	/*printf("Parent: SI_CODE:%d received\n", siginfo->si_code);
	printf("Parent: SI_ERRNO:%d received\n", siginfo->si_errno);
	printf("Parent: SI_SIGNO:%d received\n", siginfo->si_signo);*/
}

void parentProcess(pid_t child_pid)
{
	//This is a parent process
	//signal(SIGUSR1, &parentSigHandler);
	struct sigaction action;
	int sa_flags = 0;
	sa_flags |= SA_SIGINFO;

	action.sa_sigaction = &parentSigHandler;
	action.sa_flags = SA_SIGINFO;
	sigemptyset(&(action.sa_mask));
	sigaction(SIGUSR1, &action, NULL);

	printf("Parent: My PID is %d\n", getpid());
	printf("Parent: Child's PID is %d\n", child_pid);
	printf("Parent: Waiting for a signal USR1\n");

	/* Дожидаемся завершения дочернего процесса. */
	int status;
	while(1)
	{
		waitpid(child_pid, &status, 0);
		if(WIFEXITED(status))
		{
			break;
		}
	}
	printf("Parent: Child process is over\n");
}

void childProcess(pid_t parent_pid)
{
	printf("Child: My PID is %d\n", getpid());
	printf("Child: Parent's PID is %d\n", parent_pid);
	kill(parent_pid, SIGUSR1);
	union sigval value;
	value.sival_int = 0;
	int i;
	for(i = 0; i < 5; i++)
	{
		value.sival_int = i;
		printf("Child: Sending UNIX message %d to parent.\n", i);
		sigqueue(parent_pid, SIGUSR1, value);
		//usleep(500000);
		//usleep(1);
	}
}

int main(void)
{
	pid_t child_pid;
	pid_t parent_pid = getpid();

	child_pid = fork();
	if(0 != child_pid)
	{
		parentProcess(child_pid);
	}
	else
	{
		childProcess(parent_pid);
	}

	return 0;
}

