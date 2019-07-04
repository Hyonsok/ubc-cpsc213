#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

queue_t      pending_read_queue;
unsigned int sum = 0;
volatile int num_pending_reads = 0;

// To initialize the disk to use an interrupt handler
void interrupt_service_routine () {
  void* val;
  void (*callback)(void*,void*);
  queue_dequeue (pending_read_queue, &val, NULL, &callback);
  callback (val, NULL);
}

void handle_read (void* resultv, void* not_used) {
  // TODO add result to sum

  int* tmp = resultv;
  sum += *tmp;
  num_pending_reads--;
}

int main (int argc, char** argv) {

  // Command Line Arguments
  static char* usage = "usage: aRead num_blocks";
  int num_blocks;
  char *endptr;
  if (argc == 2)
    num_blocks = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // create an array of integers with the size of numblocks
  // use this array to hold onto the result

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();

  // array to store results
  int results[num_blocks];
  num_pending_reads = num_blocks;

  // Sum Blocks
  // TODO
  for (int blockno = 0; blockno < num_blocks; blockno++) {
      queue_enqueue (pending_read_queue, &results[blockno], NULL ,handle_read);
      disk_schedule_read(&results[blockno],blockno);
  }



  while (num_pending_reads > 0); // inifinite loop so that main doesn't return before all of the reads complete
  printf ("%d\n", sum);
}


