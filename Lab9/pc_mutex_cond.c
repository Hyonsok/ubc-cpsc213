#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define MAX_ITEMS      10
#define NUM_ITERATIONS 200
#define NUM_CONSUMERS  2
#define NUM_PRODUCERS  2
#define NUM_THREADS    (NUM_CONSUMERS + NUM_PRODUCERS)

int producer_wait_count;     // # of times producer had to wait
int consumer_wait_count;     // # of times consumer had to wait
int histogram [MAX_ITEMS+1]; // histogram [i] == # of times list stored i items

struct Pool* p;


struct Pool {
    int items;
    uthread_mutex_t mx;
    uthread_cond_t moreItems;
    uthread_cond_t moreSpace;
};

struct Pool* createPool() {
    struct Pool* pool = malloc (sizeof (struct Pool));
    pool->mx    = uthread_mutex_create();
    pool->moreItems = uthread_cond_create (pool->mx);
    pool->moreSpace  = uthread_cond_create (pool->mx);
    pool->items    = 0;
    return pool;
}

void produce() {
  // TODO
    uthread_mutex_lock(p->mx);
    while (p->items==MAX_ITEMS) {
        producer_wait_count ++;
        uthread_cond_wait(p->moreSpace);
    }


    if (p->items < MAX_ITEMS) {
        uthread_cond_signal(p->moreItems);
     }

    p->items++;
    histogram [p->items] += 1;
    uthread_mutex_unlock(p->mx);
}

void consume(void) {
  // TODO
    uthread_mutex_lock(p->mx);
    while (p->items == 0) {
        consumer_wait_count ++;
        uthread_cond_wait(p->moreItems);
    }

    if (p->items > 0) {
        uthread_cond_signal (p->moreSpace);
    }

  p->items--;
  histogram [p->items] += 1;
  uthread_mutex_unlock (p->mx);
}


void *producer(void *a) {
  for (int i=0; i < NUM_ITERATIONS; i++)
    produce();
  return 0;
}

void *consumer(void *a) {
  for (int i=0; i< NUM_ITERATIONS; i++)
    consume();
  return 0;
}

int main (int argc, char** argv) {
  uthread_t t[NUM_THREADS];
  uthread_init(NUM_THREADS);
  p = createPool();
  // TODO: Create Threads and Join


  for (int i = 0; i < NUM_PRODUCERS; i++) {
    t[i] = uthread_create(producer, 0);
  }

  for (int i = NUM_PRODUCERS; i < NUM_THREADS; i++) {
    t[i] = uthread_create(consumer, 0);
  }

  for (int i = NUM_PRODUCERS; i<NUM_THREADS; i++) {
    uthread_join(t[i],0);
  }

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    uthread_join(t[i],0);
  }

  printf("Producer wait: %d\nConsumer wait: %d\n\n",
         producer_wait_count, consumer_wait_count);
  
  printf ("Items value histogram:\n");
  int sum = 0;
  for (int i = 0; i <= MAX_ITEMS; i++) {
    printf ("  items=%2d, %4d times\n", i, histogram [i]);
    sum += histogram [i];
  }
  assert (sum == NUM_THREADS * NUM_ITERATIONS);
}
