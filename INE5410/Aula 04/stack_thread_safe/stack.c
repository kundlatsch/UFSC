#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

mystack_t * stack_alloc(int max_size) {
  mystack_t *s = (mystack_t *) malloc(sizeof(mystack_t));

  s->max_size = max_size;
  s->data = (int *) malloc(sizeof(int) * max_size);
  s->top = 0;
  pthread_mutex_init(&s->lock, NULL);

  return s;
}

void stack_free(mystack_t *s) {
  free(s->data);
  free(s);
  pthread_mutex_destroy(&s->lock);
}

int stack_isempty(mystack_t *s) {
  if(s->top == 0)
    return 1;
  return 0;
}

int stack_isfull(mystack_t *s) {
  if(s->top == s->max_size)
    return 1;
  return 0;
}

int stack_pop(mystack_t *s, int *ret) {
  pthread_mutex_lock(&s->lock);
  if(!stack_isempty(s)) {
    *ret = s->data[--s->top];
    pthread_mutex_unlock(&s->lock);
    return 1;
  }
  pthread_mutex_unlock(&s->lock);
  return 0;
}

int stack_push(mystack_t *s, int data) {
  pthread_mutex_lock(&s->lock);
  if(!stack_isfull(s)) {
    s->data[s->top++] = data;
    pthread_mutex_unlock(&s->lock);
    return 1;
  }
  pthread_mutex_unlock(&s->lock);
  return 0;
}
