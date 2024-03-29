//
// This is the solution to CPSC 213 Assignment 10.
// Do not distribute this code or any portion of it to anyone in any way.
// Do not remove this comment.
//

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_sem.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

struct Agent {
  uthread_sem_t match;
  uthread_sem_t paper;
  uthread_sem_t tobacco;
  uthread_sem_t smoke;
  uthread_sem_t done;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->paper   = uthread_sem_create (0);
  agent->match   = uthread_sem_create (0);
  agent->tobacco = uthread_sem_create (0);
  agent->smoke   = uthread_sem_create (0);
  agent->done    = uthread_sem_create (0);
  return agent;
}

struct Pusher {
  uthread_sem_t mutex;
  uthread_sem_t matchPaper;
  uthread_sem_t matchTobacco;
  uthread_sem_t paperTobacco;
  struct Agent*        agent;
  int                  paper, match, tobacco;
};

struct Pusher* createPusher (struct Agent* agent) {
  struct Pusher* pusher = malloc (sizeof (struct Pusher));
  pusher->mutex        = uthread_sem_create (1);
  pusher->matchPaper   = uthread_sem_create (0);
  pusher->matchTobacco = uthread_sem_create (0);
  pusher->paperTobacco = uthread_sem_create (0);
  pusher->agent        = agent;
  pusher->paper        = 0;
  pusher->match        = 0;
  pusher->tobacco      = 0;
  return pusher;
}

enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

int signal_count [5];
int smoke_count  [5];

void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};
  
  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int r = random() % 3;
    signal_count [matching_smoker [r]] ++;
    int c = choices [r];
    if (c & MATCH) {
      VERBOSE_PRINT ("match available\n");
      uthread_sem_signal (a->match);
    }
    if (c & PAPER) {
      VERBOSE_PRINT ("paper available\n");
      uthread_sem_signal (a->paper);
    }
    if (c & TOBACCO) {
      VERBOSE_PRINT ("tobacco available\n");
      uthread_sem_signal (a->tobacco);
    }
    VERBOSE_PRINT ("agent is waiting\n");
    uthread_sem_wait (a->smoke);
  }
  uthread_sem_signal (a->done);
  return NULL;
}

void pusher (struct Pusher* p, enum Resource r, uthread_sem_t sem, int* isAvailable) {
  while (1) {
    VERBOSE_PRINT ("waiting for %s\n", resource_name [r]);
    uthread_sem_wait (sem);
    VERBOSE_PRINT ("have %s\n", resource_name [r]);
    uthread_sem_wait (p->mutex);
      *isAvailable = 1;
      if (p->match && p->paper) {
        p->match = p->paper = 0;
        VERBOSE_PRINT ("signalling smoker with match and paper\n");
        uthread_sem_signal (p->matchPaper);
      } else if (p->match && p->tobacco) {
        p->match = p->tobacco = 0;
        VERBOSE_PRINT ("signalling smoker with match and tobacco\n");
        uthread_sem_signal (p->matchTobacco);
      } else if (p->paper && p->tobacco) {
        p->paper = p->tobacco = 0;
        VERBOSE_PRINT ("signalling smoker with paper and tobacco\n");
        uthread_sem_signal (p->paperTobacco);
      }
    uthread_sem_signal (p->mutex);
  }
}

void* matchPusher (void* pv) {
  struct Pusher* p = pv;
  pusher (p, MATCH, p->agent->match, &p->match);
  return NULL;
}

void* paperPusher (void* pv) {
  struct Pusher* p = pv;
  pusher (p, PAPER, p->agent->paper, &p->paper);
  return NULL;
}

void* tobaccoPusher (void* pv) {
  struct Pusher* p = pv;
  pusher (p, TOBACCO, p->agent->tobacco, &p->tobacco);
  return NULL;
}

void smoke (struct Pusher* p, enum Resource r, uthread_sem_t sem) {
  while (1) {
    VERBOSE_PRINT ("smoker with %s is waiting\n", resource_name [r]);
    uthread_sem_wait (sem);
    VERBOSE_PRINT ("smoker with %s is smoking\n", resource_name [r]);
    uthread_sem_wait (p->mutex);
      smoke_count [r] ++;
    uthread_sem_signal (p->mutex);
    uthread_sem_signal (p->agent->smoke);
  }
}

void* smokeWithMatch (void* pv) {
  struct Pusher* p = pv;
  smoke (p, MATCH, p->paperTobacco);
  return NULL;
}

void* smokeWithPaper (void* pv) {
  struct Pusher* p = pv;
  smoke (p, PAPER, p->matchTobacco);
  return NULL;
}

void* smokeWithTobacco (void* pv) {
  struct Pusher* p = pv;
  smoke (p, TOBACCO, p->matchPaper);
  return NULL;
}

void mysrandomdev() {
  unsigned long seed;
  int f = open ("/dev/random", O_RDONLY);
  read    (f, &seed, sizeof (seed));
  close   (f);
  srandom (seed);
}

int main (int argc, char** argv) {
  uthread_init (1);
  struct Agent*  a = createAgent();
  struct Pusher* p = createPusher (a);
  mysrandomdev();
  uthread_detach (uthread_create (matchPusher,      p));
  uthread_detach (uthread_create (paperPusher,      p));
  uthread_detach (uthread_create (tobaccoPusher,    p));
  uthread_detach (uthread_create (smokeWithMatch,   p));
  uthread_detach (uthread_create (smokeWithPaper,   p));
  uthread_detach (uthread_create (smokeWithTobacco, p));
  uthread_detach (uthread_create (agent,            a));
  uthread_sem_wait (a->done);
  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);
  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}
