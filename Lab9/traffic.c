#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

#define MAX_OCCUPANCY          3
#define NUM_ITERATIONS         100
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
  // TODO
  uthread_mutex_t mutex;
  uthread_cond_t pairFlagger[2];
  int occupantCount;
  int waitToCross;
  int twoDirection[2];
  int carwaiting;
  enum Direction direction;
} Street;

void initializeStreet(void) {
  // TODO
  struct Street* street = malloc (sizeof (struct Street));
  street->mutex = uthread_mutex_create();
  street->pairFlagger[NORTH] = uthread_cond_create(street->mutex);
  street->pairFlagger[SOUTH] = uthread_cond_create (street->mutex);
  street->occupantCount = 0;
  street->waitToCross = 0;
  street->twoDirection[NORTH] = 0;
  street->twoDirection[SOUTH] = 0;
  street->carwaiting = 0;
  return street;
}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_CARS)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_mutex_t waitingHistogramMutex;
int             occupancyHistogram [2] [MAX_OCCUPANCY + 1];


void enterStreet (enum Direction g) {
  // TODO
  uthread_mutex_lock(Street.mutex);
  int currentTime;
}

void leaveStreet(void) {
  // TODO
}

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogramMutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogramMutex);
}

//
// TODO
// You will probably need to create some additional procedures etc.
//


int main (int argc, char** argv) {
  uthread_init (NUM_CARS);
  initializeStreet();
  uthread_t pt [NUM_CARS];
  waitingHistogramMutex = uthread_mutex_create ();

  // TODO
  for (int i = 0; i<NUM_CARS; i++) {
    pt[i] = uthread_create (waitingHistogram,street);
  }
  for (int i = 0; i<NUM_CARS; i++) {
    uthread_join (pt[i],0);
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
