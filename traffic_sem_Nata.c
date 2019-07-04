#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_sem.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

#define MAX_OCCUPANCY          3
#define NUM_ITERATIONS         1000
#define NUM_CARS               20
#define FAIR_WAITING_COUNT     4

// These times determine the number of times yield is called when in
// the street, or when waiting before crossing again.
#define CROSSING_TIME             20
#define WAIT_TIME_BETWEEN_CROSSES 20

/**
 * You might find these declarations useful.
 */
enum Direction {NORTH = 0, SOUTH = 1};
const static enum Direction oppositeEnd [] = {SOUTH, NORTH};

struct Street {
  uthread_sem_t isEmpty;
  uthread_sem_t mutex;
  uthread_sem_t dirs[2];
  enum Direction dir;
  int counter;
  int cars;
  int waitingCars[2];
} Street;

void initializeStreet(void) {
  // TODO
  Street.mutex = uthread_sem_create(1);
  Street.isEmpty = uthread_sem_create(0);
  Street.dirs[0] = uthread_sem_create(0);
  Street.dirs[1] = uthread_sem_create(0);
  Street.waitingCars[0] = 0;
  Street.waitingCars[1] = 0;
  Street.dir = 0;
  Street.counter = 0;
  Street.cars = 0;
}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_CARS)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_sem_t   waitingHistogramMutex;
int             occupancyHistogram [2] [MAX_OCCUPANCY + 1];

void recordWaitingTime (int waitingTime) {
  uthread_sem_wait (waitingHistogramMutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_sem_signal (waitingHistogramMutex);
}

void recordEntry(enum Direction g) {
  assert(Street.cars < MAX_OCCUPANCY);
  Street.dir = g;
  Street.cars++;
  Street.counter++;
  occupancyHistogram[g][Street.cars]++;
}

void enterStreet (enum Direction g) {
  uthread_sem_wait(Street.mutex);
  enum Direction opp = oppositeEnd[g];
  // if street is not full and cars are not going the other way, or if street is empty,
  // our car can go through
  if ((Street.dir == g && Street.cars < MAX_OCCUPANCY
  && !Street.waitingCars[opp] ) || Street.cars == 0) {
        recordEntry(g);
        uthread_sem_signal(Street.mutex);
  } else {
    // or else we wait
    Street.waitingCars[g]++;
    uthread_sem_signal(Street.mutex);
    uthread_sem_wait(Street.dirs[g]);
  }
  // uthread_sem_wait(Street.spaces);
}

void leaveStreet(void) {
  uthread_sem_wait(Street.mutex);
  Street.cars--;
  // if (Street.cars == 0) {
  //   uthread_sem_signal(Street.isEmpty);
  // }
  // uthread_sem_signal(Street.spaces);
  enum Direction curr = Street.dir;
  enum Direction opp = oppositeEnd [Street.dir];
  if (Street.waitingCars[opp] > 0) {
    if (Street.cars == 0) {
      int i = 0;
      while (i < Street.waitingCars[opp] && i < MAX_OCCUPANCY) {
        i++;
        Street.waitingCars[opp]--;
        recordEntry(opp);
        uthread_sem_signal(Street.dirs[opp]);
      }
    }
  } else if (Street.waitingCars[curr]) {
    Street.waitingCars[curr]--;
    recordEntry(curr);
    uthread_sem_signal(Street.dirs[curr]);
    // uthread_sem_signal(Street.mutex);
  }
 uthread_sem_signal(Street.mutex);
}





//
// TODO
// You will probably need to create some additional procedures etc.
//

void* car(void* av) {
  enum Direction dir = random() % 2;
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int startTime = Street.counter;
    enterStreet((dir + i) % 2);
    // boi got in
    int waitTime = Street.counter - startTime - 1;
    recordWaitingTime(waitTime);
    for (int j = 0; j < NUM_CARS; j++) {
      uthread_yield();
    }
    // leave now
    leaveStreet();
    for (int j = 0; j < NUM_CARS; j++) {
      uthread_yield();
    }
  }
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (NUM_CARS);
  initializeStreet();
  uthread_t pt [NUM_CARS];
  waitingHistogramMutex = uthread_sem_create (1);

  // TODO

  for (int i = 0; i < NUM_CARS; i++) {
    pt[i] = uthread_create(car, 0);
  }
  for (int i = 0; i < NUM_CARS; i++) {
    uthread_join(pt[i], 0);
  }

  printf ("Times with 1 car  going north: %d\n", occupancyHistogram [NORTH] [1]);
  printf ("Times with 2 cars going north: %d\n", occupancyHistogram [NORTH] [2]);
  printf ("Times with 3 cars going north: %d\n", occupancyHistogram [NORTH] [3]);
  printf ("Times with 1 car  going south: %d\n", occupancyHistogram [SOUTH] [1]);
  printf ("Times with 2 cars going south: %d\n", occupancyHistogram [SOUTH] [2]);
  printf ("Times with 3 cars going south: %d\n", occupancyHistogram [SOUTH] [3]);

  printf ("Waiting Histogram\n");
  for (int i=0; i<WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Cars waited for           %4d car%s to enter: %4d time(s)\n",
	      i, i==1 ? " " : "s", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Cars waited for more than %4d cars to enter: %4d time(s)\n",
	    WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
