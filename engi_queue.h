#pragma once

#include "common.h"

typedef struct engi_queue_cont_s
{
	void *data;
	struct engi_queue_cont_s *next;
	struct engi_queue_cont_s *prev;
} engi_queue_cont_t;

typedef struct engi_queue_s
{
	engi_queue_cont_t *tail;
	engi_queue_cont_t *head;
	size_t size;
	void * (*enqueue)(void *self, void *data);
	void * (*dequeue)(void *self);
} engi_queue_t;

int engi_queue_init(engi_queue_t *self);
int engi_queue_destroy(engi_queue_t *self);

void * engi_queue_enqueue(void *self, void *data);
void * engi_queue_dequeue(void *self);


