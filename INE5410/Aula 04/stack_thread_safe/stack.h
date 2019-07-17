#ifndef _STACK
#define _STACK
#include <pthread.h>

typedef struct {
  int *data;
  int max_size;
  int top;
  pthread_mutex_t lock;
} mystack_t;

mystack_t * stack_alloc(int max_size);
void stack_free(mystack_t *s);
int stack_isempty(mystack_t *s);
int stack_isfull(mystack_t *s);
int stack_pop(mystack_t *s, int *ret);
int stack_push(mystack_t *s, int data);

#endif
