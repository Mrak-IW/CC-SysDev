#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*Compile and link with -pthread key. It is essential.*/

struct threadParams
{
	int count;
};

void* threadFunction(void *paramStructPtr)
{
    int sum = 0;
    struct threadParams *param = paramStructPtr;
    if(param->count > 0)
	{
		for(int i = 1; i <= param->count; i++)
		{
			sum += i;
		}
	}
	else
	{
		for(int i = -1; i >= param->count; i--)
		{
			sum += i;
		}
	}
	return sum;
}

int main()
{
	size_t stackSize = 1024;
	struct threadParams param;
	unsigned char* threadStack = malloc(stackSize);
	void* threadReturn;
	pthread_attr_t threadAttrs;
	pthread_t threadHandle;

	for(int i = 0; i < 256; i++)
	{
		((int *)threadStack)[i] = i;
	}

	pthread_attr_init(&threadAttrs);
	pthread_attr_setstack(&threadAttrs, threadStack, stackSize);

	param.count = 5;

	pthread_create(&threadHandle, &threadAttrs, &threadFunction, &param);
	pthread_join(threadHandle, &threadReturn);

    printf("Thread was started with arg = %d. Result = %d\n", param.count, threadReturn);

    free(threadStack);
    return 0;
}
