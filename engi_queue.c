#include <stddef.h>
#include <errno.h>

#include "engi_queue.h"

int engi_queue_init(engi_queue_t *self)
{
	self->enqueue = engi_queue_enqueue;
	self->dequeue = engi_queue_dequeue;

	self->back = NULL;
	self->front = NULL;

	self->size = 0;

	return 0;
}


int engi_queue_destroy(engi_queue_t *self)
{
	void *ret = NULL;

	self->enqueue = NULL;

	while(likely((ret = self->dequeue(self, NULL)) != NULL));

	self->dequeue = NULL;

	return 0;
}

void * engi_queue_enqueue(void *self, void *data)
{
	engi_queue_t *queue = (engi_queue_t *)self;

	engi_queue_cont_t *new = calloc(1, sizeof(engi_queue_cont_t));

	if(unlikely(new == NULL))
	{
		errno = ENOMEM;
		return NULL;
	}

	new->data = data;

	if(unlikely(queue->back == NULL))
	{
		queue->back = new;
		queue->front = new;
		goto SKIP;
	}

	queue->back->prev = new;
	SKIP:
	new->next = queue->back;
	queue->back = new;

	queue->size++;

	return NULL;
}

void * engi_queue_dequeue(void *self, void *data)
{
	engi_queue_t *queue = (engi_queue_t *)self;

	engi_queue_cont_t *old = queue->front;

	if(unlikely(old == NULL))
	{
		queue->size--;
		errno = EFAULT;
		return NULL;
	}

	engi_queue_cont_t *prev = old->prev;

	if(unlikely(prev == NULL))
	{
		queue->front = NULL;
		queue->back = NULL;
		errno = EFAULT;
		goto END;
	}

	prev->next = NULL;

	queue->front = prev;

	if(unlikely(old->prev != NULL))
	{
		old->prev = NULL;
	}
	if(data != NULL)
	{
		data = old->data;
	}

	END:;
	void *ret = old->data;

	queue->size--;

	free(old);

	return ret;
}

