#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define MAX_THINKING_TIME 25000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

typedef struct fork {
  uthread_mutex_t lock;
  uthread_cond_t forfree;
  long free;
} fork_t;

int num_phils, num_meals;    
fork_t *forks;

void nap() {
  usleep(random() % MAX_THINKING_TIME);
}

void initfork(int i) {
  forks[i].lock    = uthread_mutex_create();
  forks[i].forfree = uthread_cond_create(forks[i].lock);
  forks[i].free    = 1;
}

long getfork(long i) {
  /* TO BE IMPLEMENTED BY THE STUDENTS. */
       uthread_mutex_lock(forks[i].lock);
       while (!forks[i].free) {
            uthread_cond_wait(forks[i].forfree);
       }
       printf("philosopher %ld got the fork.\n",i);

       return 1;
}

void putfork(long i) {
  /* TO BE IMPLEMENTED BY THE STUDENTS. */
  printf("philosopher %ld will put down the fork\n", i);
  forks[rightfork(i)].free = 1;
  forks[leftfork(i)].free = 1;
  uthread_cond_signal(forks[rightfork(i)].forfree);
  uthread_mutex_unlock(forks[rightfork(i)].lock);
  uthread_cond_signal(forks[leftfork(i)].forfree);
  uthread_mutex_unlock(forks[leftfork(i)].lock);
}

int leftfork(long i) {
  return i;
}

int rightfork(long i) {
  return (i + 1) % num_phils;
}

void *phil_thread(void *arg) {
  uintptr_t id = (uintptr_t) arg;
  int meals = 0;

  while (meals < num_meals) {
    /* TO BE IMPLEMENTED BY THE STUDENTS. */
    if (id < rightfork(id)) {
        getfork(rightfork(id));
        nap();
    }
    else if (id > rightfork(id)) {
        getfork(leftfork(id));
        nap();
    }
    else {
        getfork(rightfork(id));
        nap();
    }
    putfork(id);
    nap();
    num_meals--;
  }
  return 0;
}

int main(int argc, char **argv) {

  uthread_t *p;
  uintptr_t i;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s num_philosophers num_meals\n", argv[0]);
    return 1;
  }

  num_phils = strtol(argv[1], 0, 0);
  num_meals = strtol(argv[2], 0, 0);

  forks = malloc(num_phils * sizeof(fork_t));
  p = malloc(num_phils * sizeof(pthread_t));

  uthread_init(num_phils);

  srandom(time(0));
  for (i = 0; i < num_phils; ++i) {
        initfork(i);
  }

  /* TODO: Create num_phils threads, all calling phil_thread with a
   * different ID, and join all threads.
   */

  for (i = 0; i < num_phils; i++) {
          p[i] = uthread_create(phil_thread,0);

  }

  for (i = 0; i < num_phils; i++) {
          uthread_join(p[i],0);
  }


  return 0;
}
