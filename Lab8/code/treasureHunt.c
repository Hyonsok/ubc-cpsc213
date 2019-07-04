#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "uthread.h"
#include "queue.h"
#include "disk.h"

/* A good first step would be to read the value of the
initial block and print it. Then a good second step would
be to read the second block and print its value. Now think
about adding the counter argument to the callback (that’s
what arg is for) and counting down. Finally,
be sure that you only print the value of the
last block and that your program terminates when its done.

Note that, unlike the asynchronous read code above,
in this case the main thread uses an infinite loop to keep
it from leaving once the first request has been sent.
It is your responsibility to then exit the program once you
have identified that all required tasks have completed.
You may do that by one of the following ways:

Call exit(EXIT_SUCCESS); once the program has completed all its tasks; or
Create a variable that is modified once all tasks are completed, and change the main function's final loop to a polling loop that checks this variable.
*/


queue_t pending_read_queue;
int result;
int count;

void interrupt_service_routine() {
  // TODO
  void *val, *arg;
  void (*callback) (void*,void*);
  queue_dequeue (pending_read_queue, &val, NULL, &callback);
  callback (val, NULL);
}

void handleOtherReads (void* resultv, void* countv) {
  // TODO
  int *nextBlock = resultv;
  count--;
  if (count == 0){
    printf("%d\n", result);
    exit(EXIT_SUCCESS);
  }

  queue_enqueue (pending_read_queue, nextBlock, NULL, handleOtherReads);
  disk_schedule_read(&result, *nextBlock);
}

void handleFirstRead (void* resultv, void* countv) {
  // TODO
  int *nextBlock = resultv;
  count = *nextBlock;

  if (*nextBlock == 0){
    printf("%d\n", *nextBlock);
    exit(EXIT_SUCCESS);
  }

  queue_enqueue (pending_read_queue, nextBlock, NULL, handleOtherReads);
  disk_schedule_read(&result, *nextBlock);
}

int main (int argc, char** argv) {
  // Command Line Arguments
  static char* usage = "usage: treasureHunt starting_block_number";
  int starting_block_number;
  char *endptr;
  if (argc == 2)
    starting_block_number = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();

  // Start the Hunt
  // TODO
  queue_enqueue (pending_read_queue, &result, NULL, handleFirstRead);
  disk_schedule_read(&result, starting_block_number);

  while (1){
    //printf("%d\n", results[0]);
  } // inifinite loop so that main doesn't return before hunt completes
}
