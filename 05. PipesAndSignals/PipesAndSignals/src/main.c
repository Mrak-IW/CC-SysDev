#include <stdio.h>
#include <signal.h>
#include "processFunctions.h"

void sighandler(int signum)
{
    printf("SIG:%d received\n", signum);
}

int main(void)
{
    signal(SIGUSR1, &sighandler);
    printf("Waiting for a signal USR1\n");
    pause();
    firstProcessFunction();
    secondProcessFunction();
    return 0;
}

