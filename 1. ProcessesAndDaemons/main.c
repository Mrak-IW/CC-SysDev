#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>

void forkchild()
{
	printf("I'm forked daemon-child! Kill me if you can!!! My PID is %d\n", (int)getpid());
	sleep(200);
	printf("Daemos says you BYE BYE!!!");
}

int main(int argc, char *argv[])
{
	pid_t child_pid;
	printf("The main program process ID is %d\n\n", (int)getpid());

	const char* systemCommand = "ls -lah";
	printf("New process using system(\"%s\"):\n", systemCommand);
	system(systemCommand);


	child_pid = fork();
	if(0 != child_pid)
	{
		//This is a parent process
		printf("Child process vas been forked. Its PID is %d\n", child_pid);
	}
	else
	{
		//This is a child process
		setsid();
		forkchild();
		return 0;
	}

	//spawn() is not implemented in Linux


	const char* execCmd = "/bin/ls";
	const char* execParam1 = "-lah";
	printf("New process using execle(\"%s\", \"%s\", %p):\n\t", execCmd, execParam1,  NULL);
	int ret = execle(execCmd, execParam1, NULL);
	printf("Вот эта строка у меня не выводится. Интересно, почему?\n");
	printf("Process exited with code '%d: %s'\n", errno, strerror(errno));

	return 0;
}
