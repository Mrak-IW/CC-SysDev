#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_FIRST 0

/*I don't know, why I have to copy this struct here. Really.*/
struct msgbuf
{
	__syscall_slong_t mtype;	/* type of received/sent message */
	char mtext[1];		/* text of the message */
};

int segment_id;
char *shared_memory;
struct shmid_ds shmbuffer;
int segment_size;
const int shared_segment_size = 0x6400;

const char *procName = "Parent";
const unsigned int BUFSIZE = 255;

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

void waitForSignal(const char *procName, const char *description)
{
	while(!ready)
	{
		printf("%s: Waiting for sinal [%s]\n", procName, description);
		usleep(100000);
	}
	ready = false;
}

void msgReceive(int msqid, const char *procName)
{
	char msgBuf[BUFSIZE];
	struct msgbuf *msgp = (void *)msgBuf;
	if(-1 != msgrcv(msqid, msgp, BUFSIZE - sizeof(struct msgbuf), MSG_FIRST, MSG_NOERROR))
	{
		printf("%s: Received message - '%s'\n", procName, msgp->mtext);
	}
	else
	{
		printf("%s: Can't receive the message from queue\n", procName);
		printf("%s: errno = %d (%s)\n", procName, errno, strerror(errno));
	}
}

void msgSend(int msqid, const char *procName, const char *msgtext)
{
	char msgBuf[BUFSIZE];
	struct msgbuf *msgp = (void *)msgBuf;

	sprintf(msgp->mtext, "%s", msgtext);
	msgp->mtype = 1;

	if(-1 != msgsnd(msqid, msgp, BUFSIZE - sizeof(struct msgbuf), MSG_NOERROR))
	{
		printf("%s: Message send - '%s'\n", procName, msgp->mtext);
	}
	else
	{
		printf("%s: Can't send the message via queue\n", procName);
		printf("%s: errno = %d (%s)\n", procName, errno, strerror(errno));
	}
}

void parentProcess(pid_t child_pid)
{
	//Waiting for any signal to strat
	signal(SIGUSR1, &parentSigHandler);
//	wait(NULL);
	waitForSignal(procName, "shared memory test is ready");

	//Attachin shared memory
	printf("%s: Shared memory segment_id: %d \n", procName, segment_id);
	shared_memory = (char *)shmat(segment_id, 0, 0);
	if(-1 != (int)shared_memory)
	{
		int *msqidptr = ((int *)shared_memory);
		printf("%s: Shared memory attached at address: %p \n", procName, shared_memory);

		// Discovering segment size
		shmctl (segment_id, IPC_STAT, &shmbuffer);
		segment_size = shmbuffer.shm_segsz;
		printf("%s: Segment size: %d \n", procName, segment_size);

		printf("%s: String in shared memory: '%s'\n", procName, shared_memory);

		//Let's create message queue
		*msqidptr = msgget(IPC_PRIVATE, (IPC_CREAT | IPC_EXCL | 0600));
		if(-1 == *msqidptr)
		{
			printf("%s: Can't create message queue\n", procName);
		}
		else
		{
			printf("%s: Message queue id = %d\n", procName, *msqidptr);
		}

		printf("%s: Sending a signal [queue is ready]\n", procName);
		kill(child_pid, SIGUSR1);
		waitForSignal(procName, "ack");

		msgSend(*msqidptr, procName, "Are you hearing me?");
		msgReceive(*msqidptr, procName);
		msgSend(*msqidptr, procName, "I can hear you too");

		waitForSignal(procName, "end of conversation");

		//Delete the queue
		struct msqid_ds buf;
		printf("%s: Deleting the queue id[%d]\n", procName, *msqidptr);
		if(-1 == msgctl(*msqidptr, IPC_RMID, &buf))
		{
			printf("%s: Can't remove message queue id[%d]\n", procName, *msqidptr);
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
	printf("%s: Shared memory segment_id: %d \n", procName, segment_id);
	shared_memory = (char *)shmat(segment_id, 0, 0);
	printf("%s: Shared memory attached at address: %p \n", procName, shared_memory);

	// Discovering segment size
	shmctl (segment_id, IPC_STAT, &shmbuffer);
	segment_size = shmbuffer.shm_segsz;
	printf("%s: Segment size: %d \n", procName, segment_size);

	sprintf(shared_memory, "Test string.");

	printf("%s: Sending a signal [shared memory test is ready]\n", procName);
	kill(parent_pid, SIGUSR1);

	waitForSignal(procName, "queue is ready");

	printf("%s: I know that the queue is ready\n", procName);
	int *msqidptr = ((int *)shared_memory);
	printf("%s: Message queue id = %d\n", procName, *msqidptr);

	printf("%s: Sending a signal [ack]\n", procName);
	kill(parent_pid, SIGUSR1);

	msgReceive(*msqidptr, procName);
	msgSend(*msqidptr, procName, "Loud and Clear");
	msgReceive(*msqidptr, procName);

	printf("%s: Sending a signal [end of conversation]\n", procName);
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

