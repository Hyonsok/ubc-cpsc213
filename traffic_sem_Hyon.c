
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
  uthread_sem_t done;
  uthread_sem_t canEnter[2];
  int           occupantCount;
  enum Direction occupantDirection;
  int waitersCount[2];
} Street;

void initializeStreet(void) {
  // TODO
  struct Street* street = malloc (sizeof (struct Street));
  street -> mutex       = uthread_sem_create(1);
  street -> done        = uthread_sem_create(0);
  street -> canEnter [NORTH]    = uthread_sem_create(0);
  street -> canEnter [SOUTH]    = uthread_sem_create(0);
  street -> occupantCount       = 0;
  street -> occupantDirection   = 0;
  street -> waitersCount [NORTH] = 0;
  street -> waitersCount [SOUTH] = 0;
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

void recordEntry (struct Street* street, enum Direction g) {
    assert (street -> occupantCount == 0 || street -> occupantDirection == g);
    assert (street -> occupantCount < MAX_OCCUPANCY);
    street -> occupantCount ++;
    street -> occupantDirection = g;
    entryTicker;
    occupancyHistogram [street->occupantDirection] [street -> occupantCount] ++;
}


void enterStreet (struct Street* street, enum Direction g) {
  // TODO
  uthread_sem_wait(street->mutex);
  int isEmpty   = street -> occupantCount == 0;
  int hasRoom   = street -> occupantCount < MAX_OCCUPANCY;
  int sameDirection = street -> occupantDirection == g;
  int otherDirectionWaiting = street -> waitersCount [oppositeEnd [g]] > 0;
  int canEnter = (isEmpty || (hasRoom && sameDirection && !otherDirectionWaiting));
  if (canEnter) {
    recordEntry (street, g);
  }
  else {
    street->waitersCount [g] ++;
  }
  uthread_sem_signal (street -> mutex);
  if (!canEnter) {
    uthread_sem_wait (street -> canEnter [g]);
  }
}

void leaveStreet(struct Street* street) {
  // TODO
  uthread_sem_wait (street -> mutex);
    street -> occupantCount -= 1;
    enum Direction inDirection = street -> occupantDirection;
    enum Direction outDirection = oppositeEnd [inDirection];
    if (street -> waitersCount [outDirection] > 0) {
        if (street -> occupantCount == 0) {
        for (int i = 0; i < street -> waitersCount [outDirection] && i <MAX_OCCUPANCY; i++) {
                street -> waitersCount [outDirection];
                recordEntry (street,outDirection);
                uthread_sem_signal (street -> canEnter [outDirection]);
             }
        }
    } else if (street -> waitersCount [inDirection]) {
        street -> waitersCount [inDirection] --;
        recordEntry (street, inDirection);
        uthread_sem_signal (street->canEnter [inDirection]);
    }
    uthread_sem_signal (street -> mutex);
}

void recordWaitingTime (int waitingTime) {
  uthread_sem_wait (waitingHistogramMutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_sem_signal (waitingHistogramMutex);
}

void* car (void* streetv) {
    struct Street* street = streetv;
    enum Direction Direction = random() & 1;

    for (int i = 0; i<NUM_ITERATIONS; i++) {
        int startTime = entryTicker;
        enterStreet (street,Direction);
        recordWaitingTime (entryTicker - startTime - 1);
        for (int i = 0; i < NUM_CARS; i++) {
            uthread_yield();
        }
        leaveStreet(street);
        for (int i = 0; i <NUM_CARS; i++) {
            uthread_yield();
        }

    }
    uthread_sem_signal (street->done);
    return NULL;
}

int main (int argc, char** argv) {
  uthread_init (NUM_CARS);
  initializeStreet();
  uthread_t pt [NUM_CARS];
  waitingHistogramMutex = uthread_sem_create (1);
  struct Street* street = createStreet();

  // TODO
  for (int i = 0; i<NUM_CARS; i++) {
    uthread_detach(uthread_create(car,street));
  }
  for (int i = 0; i < NUM_CARS; i++) {
    uthread_sem_wait (street->done);
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
