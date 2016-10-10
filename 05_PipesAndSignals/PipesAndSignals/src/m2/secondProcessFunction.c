#include "processFunctions.h"

void secondProcessFunction(int pipeEnds[2])
{
	printf("Child: I'm Process the Second\n");

	/* Это дочерний процесс. Закрываем копию входного конца
	канала */

	close(pipeEnds[1]);

	/* Соединяем выходной конец канала со стандартным входным
	потоком. */

	dup2(pipeEnds[0], STDIN_FILENO);

	char buf[255];
	memset(buf, 0, 255);
	read(STDIN_FILENO, buf, 254);

	printf("Child: Received: %s\n", buf);
}
