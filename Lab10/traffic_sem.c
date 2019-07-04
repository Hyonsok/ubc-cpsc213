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
  uthread_sem_t mutex;
  uthread_sem_t isEmpty;
  uthread_sem_t directions[2];
  int           occupantCount;
  enum Direction occupantDirection;
  int waitingCars[2];
  int numberOfCars;
} Street;

void initializeStreet(void) {
  // TODO
  Street.mutex       = uthread_sem_create(1);
  Street.isEmpty       = uthread_sem_create(0);
  Street.directions [NORTH]    = uthread_sem_create(0);
  Street.directions [SOUTH]    = uthread_sem_create(0);
  Street.occupantCount       = 0;
  Street.occupantDirection   = 0;
  Street.waitingCars [NORTH] = 0;
  Street.waitingCars [SOUTH] = 0;
  Street.numberOfCars = 0;
}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_CARS)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_sem_t   waitingHistogramMutex;
int             occupancyHistogram [2] [MAX_OCCUPANCY + 1];

//
// TODO
// You will probably need to create some additional procedures etc.
//
void recordWaitingTime (int waitingTime) {
  uthread_sem_wait (waitingHistogramMutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_sem_signal (waitingHistogramMutex);
}

void recordEntry (enum Direction g) {
    assert (Street.numberOfCars < MAX_OCCUPANCY);
    Street.occupantDirection = g;
    Street.numberOfCars++;
    Street.occupantCount ++;
    occupancyHistogram [g] [Street.numberOfCars] ++;
}



void enterStreet (enum Direction g) {
  // TODO
  uthread_sem_wait(Street.mutex);
  enum Direction oppositeDirection = oppositeEnd[g];
  if ((Street.occupantDirection == g && Street.numberOfCars < MAX_OCCUPANCY
                                     && !Street.waitingCars[oppositeDirection])
                                     || Street.numberOfCars == 0) {
    recordEntry (g);
    uthread_sem_signal (Street.mutex);
  }
  else {
    Street.waitingCars[g] ++;
    uthread_sem_signal(Street.mutex);
    uthread_sem_wait (Street.directions[g]);
  }
}

void leaveStreet(void) {
  // TODO
  uthread_sem_wait (Street.mutex);
  Street.numberOfCars --;
  enum Direction inDirection = Street.occupantDirection;
  enum Direction outDirection = oppositeEnd [inDirection];
    if (Street.waitingCars [outDirection] > 0) {
        if (Street.numberOfCars == 0) {
        for (int i = 0; i < Street.waitingCars [outDirection]
                        && i <MAX_OCCUPANCY; i++) {
                Street.waitingCars [outDirection]--;
                recordEntry (outDirection);
                uthread_sem_signal (Street.directions [outDirection]);
             }
        }
    } else if (Street.waitingCars [inDirection]) {
        Street.waitingCars [inDirection] --;
        recordEntry (inDirection);
        uthread_sem_signal (Street.directions [inDirection]);
    }
    uthread_sem_signal (Street.mutex);
}



void* car (void* streetv) {
    enum Direction direction = random() & 2;
    for (int i = 0; i<NUM_ITERATIONS; i++) {
        int startTime = Street.occupantCount;
        enterStreet ((direction+i)%2);
        recordWaitingTime (Street.occupantCount - startTime - 1);
        for (int i = 0; i < NUM_CARS; i++) {
            uthread_yield();
        }
        leaveStreet();
        for (int i = 0; i <NUM_CARS; i++) {
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
  for (int i = 0; i<NUM_CARS; i++) {
    pt[i] = uthread_create (car,0);
  }
  for (int i = 0; i < NUM_CARS; i++) {
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