#include <stddef.h>
#include <errno.h>

#include "engi_queue.h"

int engi_queue_init(engi_queue_t *self)
{
	self->enqueue = engi_queue_enqueue;
	self->dequeue = engi_queue_dequeue;

	self->tail = NULL;
	self->head = NULL;

	self->size = 0;

	return 0;
}


int engi_queue_destroy(engi_queue_t *self)
{
	void *ret = NULL;

	self->enqueue = NULL;

	while(likely((ret = self->dequeue(self)) != NULL));

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

	if(unlikely(queue->tail == NULL)) // Queue is empty
	{
		queue->tail = new;
		queue->head = new;
		new->next = NULL;
		new->prev = NULL;
	}
	else // Queue is not empty
	{
		engi_queue_cont_t *old_tail = queue->tail;
		new->next = old_tail; // New node's next points to the old_tail
		new->prev = NULL;     // New node is the new tail, so its prev is NULL
		old_tail->prev = new; // Old_tail's prev points to the new node
		queue->tail = new;    // Update queue's tail pointer
	}

	queue->size++;

	return NULL; // Indicate success
}

void * engi_queue_dequeue(void *self)
{
	engi_queue_t *queue = (engi_queue_t *)self;
	engi_queue_cont_t *old_head = queue->head; // Changed from old_front
	void *user_data = NULL;

	if (old_head == NULL) { // Queue is empty
		// Per instructions, do not modify errno or size
		return NULL;
	}

	user_data = old_head->data; // Store data to return

	// Decrement size as we are sure to dequeue one item
	// This must happen before any potential modification of queue->head/tail
	// if new_head is NULL, as size is a property of the queue object itself.
	queue->size--;

	engi_queue_cont_t *new_head = old_head->prev; // 'prev' points towards tail/newer items

	if (new_head == NULL) { // This was the last item in the queue
		queue->head = NULL; // Changed from front
		queue->tail = NULL; // Changed from back
	} else {
		// new_head is now the oldest item. Its 'next' pointer (towards older items)
		// must be NULL.
		new_head->next = NULL;
		queue->head = new_head; // Changed from front
	}

	// Clean up the dequeued node's pointers (good practice)
	// old_head->next should already be NULL as it was the frontmost element.
	// If it wasn't, that would indicate a list corruption prior to this call.
	// We set old_head->prev to NULL to sever its link from the list completely.
	old_head->prev = NULL;
	// old_head->next = NULL; // Already should be NULL

	free(old_head); // Free the container node

	return user_data; // Return the user's data
}

