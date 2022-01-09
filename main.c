#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include "engi_queue.h"

#include "common.h"

int main()
{
	engi_queue_t queue = {0};
	engi_queue_init(&queue);

	for(int i = 0; i < 1025; i++)
	{
		int *temp = calloc(1, sizeof(i));

		if(temp == NULL)
		{
			exit(ENOMEM);
		}

		*temp = i;

		if(i == 256) *temp = 0xAA;

		queue.enqueue(&queue, temp);
	}

	for(int i = 0; i < 1025; i++)
	{
		int *temp = queue.dequeue(&queue, NULL);

		if(i == 256) assert(*temp == 0xAA);

		free(temp);
	}

	engi_queue_destroy(&queue);

}

