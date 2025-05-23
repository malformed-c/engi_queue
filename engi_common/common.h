#pragma once

#ifndef COMMON_H
#define COMMON_H

#define likely(x)	__builtin_expect(!!(x), 1)
#define unlikely(x)	__builtin_expect(!!(x), 0)
#define NOP(x)		asm("nop");

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
//#include <sys/socket.h>
//#include <errno.h>
//#include <netdb.h>
//#include <string.h>
//#include <ctype.h>
//#include <unistd.h>
//#include <signal.h>
//#include <netinet/in.h>
//#include <netinet/tcp.h>

#if  __GNUC__ <= 6
void * reallocarray(void *optr, size_t nmemb, size_t elem_size)
{
	size_t bytes = nmemb * elem_size;
	return realloc(optr, bytes);
}
#endif

typedef struct args_tuple_s
{
	void *self;
	void *data;
} args_tuple_t;

typedef struct str_s
{
	char *str;
	unsigned int cap;
	unsigned int len;

} str_t;

typedef struct cstr_s
{
	char str[64];
	unsigned int cap;
	unsigned int len;

} cstr_t;

typedef struct engi_endpoint_s
{

} engi_endpoint_t;

// void mutex_write(void *self, pthread_mutex_t *mtx)
// {
//
// }

#endif //COMMON_H
