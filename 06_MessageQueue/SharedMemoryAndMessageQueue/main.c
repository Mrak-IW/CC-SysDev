#include <stdio.h>
#include <sys/wait.h>
//#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <unistd.h>
//#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

int segment_id;
char *shared_memory;
struct shmid_ds shmbuffer;
int segment_size;
const int shared_segment_size = 0x6400;

const char *procName = "Parent";

bool ready = false;

void parentSigHandler(int signum)
{
	printf("Parent: SIG:%d received\n", signum);
	ready = true;
}

void childSigHandler(int signum)
{
	printf("Child: SIG:%d received\n", signum);
	ready = true;
}

void waitForSignal(char *procName)
{
	while(!ready)
	{
		printf("%s: Waiting for sinal\n", procName);
		usleep(100000);
	}
	ready = false;
}

void parentProcess(pid_t child_pid)
{
	int msqid;

	//Waiting for any signal to strat
	signal(SIGUSR1, &parentSigHandler);
//	wait(NULL);
	waitForSignal(procName);

	//Attachin shared memory
	printf("%s: Shared memory segment_id: %d \n", procName, segment_id);
	shared_memory = (char *)shmat(segment_id, 0, 0);
	if(-1 != shared_memory)
	{
		printf("%s: Shared memory attached at address: %p \n", procName, shared_memory);

		// Discovering segment size
		shmctl (segment_id, IPC_STAT, &shmbuffer);
		segment_size = shmbuffer.shm_segsz;
		printf("%s: Segment size: %d \n", procName, segment_size);

		printf("%s: received string - '%s'\n", procName, shared_memory);

		//Let's create message queue
		msqid = msgget(IPC_PRIVATE, (IPC_CREAT | IPC_EXCL | 0400));
		if(-1 == msqid)
		{
			printf("%s: Can't create message queue\n", procName);
		}
		else
		{
			printf("%s: Message queue id = %d\n", procName, msqid);
		}

		printf("%s: Sending a signal [queue is ready]\n", procName);
		kill(child_pid, SIGUSR1);
		printf("%s: Waiting for ack\n", procName);
		waitForSignal(procName);

		//Delete the queue
		struct msqid_ds buf;
		printf("%s: Deleting the queue\n", procName);
		if(-1 == msgctl(msqid, IPC_RMID, &buf))
		{
			printf("%s: Can't remove message queue id[%d]\n", procName, msqid);
		}
	}
	else
	{
		printf("%s: errno = %d (%s)\n", procName, errno, strerror(errno));
	}
	printf("%s:\tI'm tired. I'm going away.\n", procName);
}

void childProcess(pid_t parent_pid)
{
	signal(SIGUSR1, &childSigHandler);

	procName = "Child";
	//Attachin shared memory
	printf("%s: Shared memory segment_id: %p \n", procName, segment_id);
	shared_memory = (char *)shmat(segment_id, 0, 0);
	printf("%s: Shared memory attached at address: %p \n", procName, shared_memory);

	// Discovering segment size
	shmctl (segment_id, IPC_STAT, &shmbuffer);
	segment_size = shmbuffer.shm_segsz;
	printf("%s: Segment size: %d \n", procName, segment_size);

	sprintf(shared_memory, "Test string.");

	printf("%s: Sending a signal [shared memory ready]\n", procName);
	kill(parent_pid, SIGUSR1);

	waitForSignal(procName);

	printf("%s: I know that queue is ready\n", procName);
	kill(parent_pid, SIGUSR1);
}

int main(void)
{
	pid_t child_pid;
	pid_t parent_pid = getpid();

	//Creating shared memory
	segment_id = shmget(IPC_PRIVATE, shared_segment_size, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

	child_pid = fork();
	if(0 != child_pid)
	{
		parentProcess(child_pid);
		//Deleting shared memory
		printf("%s: Deleting shared memory\n", procName);
		shmctl (segment_id, IPC_RMID, 0);
	}
	else
	{
		childProcess(parent_pid);
		return 0;
	}

	return 0;
}

