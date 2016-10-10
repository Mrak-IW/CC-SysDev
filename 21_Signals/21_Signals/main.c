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
	char* sigtype = (signum == SIGUSR1 ? "UNIX" : "RT");

	printf("Parent:\tSIG:%s received\n", sigtype);
	printf("\tVAL:%d\n", value.sival_int);
	/*printf("Parent:\tSI_CODE:%d received\n", siginfo->si_code);
	printf("Parent:\tSI_ERRNO:%d received\n", siginfo->si_errno);
	printf("Parent:\tSI_SIGNO:%d received\n", siginfo->si_signo);*/
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
	sigaction(SIGRTMIN + 1, &action, NULL);

	printf("Parent:\tMy PID is %d\n", getpid());
	printf("Parent:\tChild's PID is %d\n", child_pid);
	printf("Parent:\tWaiting for a signal USR1\n");

	/* Waiting for child process ending */
	int status;
	while(1)
	{
		wait(&status);
		if(WIFEXITED(status))
		{
			break;
		}
	}
	//sleep(5);
	printf("Parent:\tI'm tired. I'm going away.\n");
}

void childProcess(pid_t parent_pid)
{
	printf("Child:\tMy PID is %d\n", getpid());
	printf("Child:\tParent's PID is %d\n", parent_pid);
	//kill(parent_pid, SIGUSR1);
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

	for(i = 0; i < 5; i++)
	{
		value.sival_int = SIGRTMIN + i;
		printf("Child: Sending RT message %d to parent.\n", SIGRTMIN + i);
		sigqueue(parent_pid, SIGRTMIN + 1, value);
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

