#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "uthread_sem.h"
#include "spinlock.h"

#define MAX_ITEMS      10
#define NUM_ITERATIONS 10000
#define NUM_CONSUMERS  2
#define NUM_PRODUCERS  2
#define NUM_THREADS    (NUM_CONSUMERS + NUM_PRODUCERS)

int histogram [MAX_ITEMS+1]; // histogram [i] == # of times list stored i items

int items = 0;

// semaphore
uthread_sem_t lock;
uthread_sem_t forData;
uthread_sem_t forSpace;

void produce(void) {
  // TODO 
  uthread_sem_wait(forSpace);
  uthread_sem_wait(lock); 
  items++;
  histogram[items] += 1;
  uthread_sem_signal(lock); 
  uthread_sem_signal(forData);
  
  assert(items <= MAX_ITEMS && 0 <= items);
}

void consume(void) {
  // TODO
  uthread_sem_wait(forData);
  uthread_sem_wait(lock); 
  items--;
  histogram[items] += 1;
  uthread_sem_signal(lock);
  uthread_sem_signal(forSpace);

  assert(items <= MAX_ITEMS && 0 <= items);
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
  lock = uthread_sem_create(1);      // lock
  forData = uthread_sem_create(0);   // empty buf
  forSpace = uthread_sem_create(10); // full buf

  // an array for threads
  uthread_t tArray[4];

  // create threads
  tArray[0] = uthread_create(producer,0);
  tArray[1] = uthread_create(producer,0);
  tArray[2] = uthread_create(consumer,0);
  tArray[3] = uthread_create(consumer,0);

  // join threads
  for (int i = 0; i < 4; i++){
    uthread_join(tArray[i], 0);
  }

  
  printf ("Items value histogram:\n");
  int sum = 0;
  for (int i = 0; i <= MAX_ITEMS; i++) {
    printf ("  items=%2d, %4d times\n", i, histogram [i]);
    sum += histogram [i];
  }
  assert (sum == NUM_THREADS * NUM_ITERATIONS);
  return 0;
}
