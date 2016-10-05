#include <stdio.h>
#include <signal.h>

void sighandler(int signum)
{
    printf("La-la-la SIG:%d\n", signum);
}

int main(void)
{
    signal(SIGUSR1, &sighandler);
    printf("Waiting for a signal USR1\n");
    pause();
    return 0;
}

