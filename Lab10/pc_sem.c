#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "uthread_sem.h"

#define MAX_ITEMS      10
#define NUM_ITERATIONS 200
#define NUM_CONSUMERS  2
#define NUM_PRODUCERS  2
#define NUM_THREADS    (NUM_CONSUMERS + NUM_PRODUCERS)

int histogram [MAX_ITEMS+1]; // histogram [i] == # of times list stored i items

int items = 0;

// semaphore
uthread_sem_t lock;
uthread_sem_t moreSpace;
uthread_sem_t moreItems;

void produce(void) {
  // TODO
  uthread_sem_wait(moreSpace);
  uthread_sem_wait(lock);
  items++;
  histogram[items] += 1;
  uthread_sem_signal (lock);
  uthread_sem_signal (moreItems);

}

void consume(void) {
  // TODO
  uthread_sem_wait (moreItems);
  uthread_sem_wait (lock);
  items --;
  histogram [items] += 1;
  uthread_sem_signal (lock);
  uthread_sem_signal (moreSpace);
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

  // TODO: Create Threads and Join
  lock = uthread_sem_create (1);
  moreSpace = uthread_sem_create(MAX_ITEMS);
  moreItems = uthread_sem_create(0);

  for (int i = 0; i < NUM_PRODUCERS; i++) {
    t[i] = uthread_create (producer,0);
  }
  for (int i = NUM_PRODUCERS; i < NUM_THREADS; i++) {
    t[i] = uthread_create (consumer,0);
  }
  for (int i = NUM_PRODUCERS; i < NUM_THREADS; i++) {
    uthread_join(t[i],0);
  }
  for (int i = 0; i<NUM_PRODUCERS; i++) {
    uthread_join(t[i],0);
  }


  printf ("Items value histogram:\n");
  int sum = 0;
  for (int i = 0; i <= MAX_ITEMS; i++) {
    printf ("  items=%2d, %4d times\n", i, histogram [i]);
    sum += histogram [i];
  }
  assert (sum == NUM_THREADS * NUM_ITERATIONS);
}
