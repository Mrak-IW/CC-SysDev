#include "processFunctions.h"

void firstProcessFunction(int pipeEnds[2])
{
	printf("Parent: I'm Process the First\n");
	/* Это родительский процесс. */

	FILE* stream;

	/* Закрываем копию выходного конца канала. */

	close(pipeEnds[0]);

	/* Приводим дескриптор входного конца канала к типу FILE*
	и записываем данные в канал. */
	stream = fdopen(pipeEnds[1], "w");

	fprintf(stream, "Hello from parent via a pipe.");
	fflush(stream);
	close(pipeEnds[1]);
}
