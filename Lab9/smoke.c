#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__)
#else
#define VERBOSE_PRINT(S, ...) ((void) 0) // do nothing
#endif

int sum = 0;
uthread_cond_t match_paper;
uthread_cond_t match_tobacco;
uthread_cond_t paper_tobacco;


struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create (agent->mutex);
  agent->match   = uthread_cond_create (agent->mutex);
  agent->tobacco = uthread_cond_create (agent->mutex);
  agent->smoke   = uthread_cond_create (agent->mutex);
  return agent;
}



/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can re-write it if you like, but be sure that all it does
 * is choose 2 random reasources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};

  srandom(time(NULL));

  uthread_mutex_lock (a->mutex);

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    int r = random() % 3;
    signal_count [matching_smoker [r]] ++;
    int c = choices [r];
    if (c & MATCH) {
      VERBOSE_PRINT ("match available\n");
      uthread_cond_signal (a->match);
    }
    if (c & PAPER) {
      VERBOSE_PRINT ("paper available\n");
      uthread_cond_signal (a->paper);
    }
    if (c & TOBACCO) {
      VERBOSE_PRINT ("tobacco available\n");
      uthread_cond_signal (a->tobacco);
    }
    VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
    uthread_cond_wait (a->smoke);
  }

  uthread_mutex_unlock (a->mutex);
  return NULL;
}
//
 // TODO
 // You will probably need to add some procedures and struct etc.
 //

 void receive_two_resources(int res) {
     switch(res) {
         case MATCH + PAPER:
               VERBOSE_PRINT ("smoker receives match and paper.\n");
               uthread_cond_signal(match_paper);
               sum = 0;
               break;
         case MATCH + TOBACCO:
               VERBOSE_PRINT ("smoker receives match and tobacco.\n");
               uthread_cond_signal(match_tobacco);
               sum = 0;
               break;
         case PAPER + TOBACCO:
               VERBOSE_PRINT ("smoker receives paper and tobacco.\n");
               uthread_cond_signal(paper_tobacco);
               sum = 0;
               break;
         default:
               break;

     }
 }
 void* tobacco_listener (void* av){
   struct Agent* a = av;
   uthread_mutex_lock(a->mutex); // must request acquiring mutex outside while loop
   while(1){
     uthread_cond_wait(a->tobacco);
     sum = sum + TOBACCO;
     receive_two_resources(sum);
   }
   uthread_mutex_unlock(a->mutex);
 }

 void* paper_listener (void* av){
   struct Agent* a = av;
   uthread_mutex_lock(a->mutex);
   while(1){
     uthread_cond_wait(a->paper);
     sum = sum + PAPER;
     receive_two_resources(sum);
   }
   uthread_mutex_unlock(a->mutex);
 }

 void* match_listener (void* av){
   struct Agent* a = av;
   uthread_mutex_lock(a->mutex);
   while(1){
     uthread_cond_wait(a->match);
     sum = sum + MATCH;
     receive_two_resources(sum);
   }
   uthread_mutex_unlock(a->mutex);
 }


 void* smoker_tobacco (void* av){
   struct Agent* a = av;
   uthread_mutex_lock(a->mutex);
   while(1){
     uthread_cond_wait(match_paper);
     VERBOSE_PRINT ("smoker has tobacco.\n");
     uthread_cond_signal(a->smoke);
     smoke_count [TOBACCO]++;
   }
   uthread_mutex_unlock(a->mutex);
 }

 void* smoker_paper (void* av) {
     struct Agent* a = av;
     uthread_mutex_lock(a->mutex);
     while (1) {
         uthread_cond_wait(match_tobacco);
         VERBOSE_PRINT("smoker has paper.\n");
         uthread_cond_signal(a->smoke);
         smoke_count [PAPER]++;
     }
     uthread_mutex_unlock (a->mutex);
 }

 void* smoker_match (void *av) {
     struct Agent* a = av;
     uthread_mutex_lock(a->mutex);
     while (1) {
         uthread_cond_wait(paper_tobacco);
         VERBOSE_PRINT("smoker has match.\n");
         uthread_cond_signal(a->smoke);
         smoke_count [MATCH]++;

     }
     uthread_mutex_unlock(a->mutex);
 }

int main (int argc, char** argv) {
  uthread_init (7);
  struct Agent*  a = createAgent();

   //TODO

  match_paper = uthread_cond_create(a->mutex);
  paper_tobacco = uthread_cond_create(a->mutex);
  match_tobacco = uthread_cond_create(a->mutex);
  uthread_create (tobacco_listener, a);
  uthread_create (paper_listener, a);
  uthread_create (match_listener, a);
  uthread_create(smoker_tobacco,a);
  uthread_create(smoker_match,a);
  uthread_create(smoker_paper, a);

  uthread_join (uthread_create (agent, a), 0);

  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);

  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}