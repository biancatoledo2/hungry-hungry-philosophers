/*
 * CPSC 351-02 Summer 2018 Session B
 * Assignment 3 Dining Philosophers
 * Michael Shafae
 * mshafae at fullerton edu
 *
 * This is the main function for a dining philosophers exercise. The
 * program is given the number of philosophers, the maximum number
 * of seconds a philosopher sleeps/thinks, and optionally a seed value
 * for a random number generator. The program prints to standard out
 * what each philosopher is doing.
 *
 * After the simulation of the dining philosopher ends, the statistics
 * for how long the total block time and the individual philosopher's
 * block time is reported.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <err.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <libgen.h>

#include "philosopher.h"
#include "utilities.h"
#define RAND_STATE_SIZE 64



/** Print out how to use the program; optionally print a message.
 * Help the user understand how the program is used through a simeple
 * usage message. Optionally, a message is printed when a non-null
 * msg is passed.
 */
void usage(char *msg){
  if(msg){
    fprintf(stderr, "Error: %s\n", msg);
  }
  fprintf(stderr, "%s -n <num_philosophers> -s <seed> -m <max_sleep> -a <appetite> -r <run_time>\n", _program_name);
  fprintf(stderr, "Example: %s -n 5 -s 1234 -m 3 -a 10 -r 60\n", _program_name);
}

/** The entry point for each philosopher thread.
 * Set up the individual thread and then loop indefinitely dining on 
 * a never ending bowl of noodles.
 */
void* philosopher_thread_entry_point(void *philo_struct){
  Philosopher* philo;
  long sleep_time, eating_time;
  long t;
  char buffer[16];
  philo = (Philosopher*)philo_struct;
  pthread_t thread_id = pthread_self( );
  sprintf(buffer, "%d", philo->id);
  pthread_setname_np(thread_id, buffer);

  while(1){
    // First the philosopher ponders the meaning of life.
    sleep_time = bounded_random(1, philo->max_sleep);
    philo->state = philo_thinking;
    philo->total_thinking_time[philo->id] += sleep_time;
    debug("Philosopher %d thinking for %ld seconds\n", philo->id, sleep_time);
    sleep(sleep_time);
    debug("Philosopher %d is no longer thinking.\n", philo->id);

    // The thinking, triggers pangs of hunger
    philo->state = philo_hungry;
    debug("Philosopher %d is hungry.\n", philo->id);
    t = time(NULL);
    pthread_mutex_lock(philo->block_monitor);
    philo->start_block_time[philo->id] = t;
    pthread_mutex_unlock(philo->block_monitor);

    debug("Philosopher %d is reaching for a chopstick.\n", philo->id);
    philosopher_chopstick_pickup(philo);
    debug("Philosopher %d is trying to eat.\n", philo->id);
    philo->state = philo_eating;

    pthread_mutex_lock(philo->block_monitor);
    philo->total_block_time[philo->id] += (time(NULL) - t);
    philo->start_block_time[philo->id] = -1;
    pthread_mutex_unlock(philo->block_monitor);

    eating_time = bounded_random(1, philo->appetite);
    philo->total_eating_time[philo->id] += eating_time;

    debug("Philosopher %d eating for %ld seconds.\n", philo->id, eating_time);
    sleep(eating_time);
    debug("Philosopher %d is trying to put down a chopstick.\n", philo->id);
    philosopher_chopstick_putdown(philo);
    debug("Philosopher %d no longer eating. Total block time so far is %d.\n", philo->id, philo->total_block_time[philo->id]);
  }

  return NULL;
}

int main(int argc, char* argv[]){
  _program_name = basename(argv[0]);
  pthread_t* threads;
  Philosopher* philos;
  char rand_state[RAND_STATE_SIZE];
  int opt = 0;
  int num_philosophers = 4;
  int seed = -1;
  int max_sleep = 5;
  int max_appetite = 10;
  int run_time = 60;
  void* philo_local_vals;
  long t0;
  int* block_time;
  int* block_starting;
  int* thinking_time;
  int* eating_time;
  pthread_mutex_t* block_monitor;

  while((opt = getopt(argc, argv, "n:s:m:a:r:")) != -1){
    switch(opt){
      case 'n':
        num_philosophers = atoi(optarg);
      break;

      case 's':
        seed = atoi(optarg);
      break;

      case 'm':
        max_sleep = atoi(optarg);
      break;

      case 'a':
        max_appetite = atoi(optarg);
      break;

      case 'r':
        run_time = atoi(optarg);
      break;

      default:
        usage("Unrecognized option; exiting.");
        exit(1);
      break;
    }
  }

  if(num_philosophers < 2){
    usage("Too few philosophers; exiting.");
    exit(1);
  }

  if(seed == -1){
    // Special case - make it hard to guess
    seed = time(NULL);
  }

  if(run_time <= 0){
    usage("Run time must be a positive value.");
    exit(1);
  }
  if(initstate(seed, rand_state, RAND_STATE_SIZE) == NULL){
    err(1, "Could not initiate random number generator state.");
  }
  debug("Starting with %d philosophers, seed %d, maximum sleep %d, and maximum appetite %d.\n", num_philosophers, seed, max_sleep, max_appetite);

  // Alocate memory for the threads
  threads = (pthread_t*)calloc(num_philosophers, sizeof(pthread_t));
  if(threads == NULL){
    err(1, "Could not allocate memory for the threads.");
  }

  // Allocate memory for the philosophers
  philos = (Philosopher*)calloc(num_philosophers, sizeof(Philosopher));
  if(philos == NULL){
    err(1, "Could not allocate memory for the philosophers.");
  }

  /*
   * Initialize the set of all philosophers.
   *
   * Each philosopher is set up and made ready for execution. The
   * implementation of void* philosopher_init_local_vals(int philo_count)
   * (see philosopher.h and philosopher.c), will return a pointer to data
   * that is unique to your implementation.
   *
   * The first block initializes data common to all philosophers. The second
   * block then assigns the common data to each individual philosopher and
   * initializes the other unique data fields for each philosopher.
   */
  /*
   * Common to all philosophers.
   */
  philo_local_vals = philosopher_init_local_vals(num_philosophers);
  // start time, iin seconds from the epoch
  t0 = time(0);
  // Arry to keep track of block time.
  block_time = (int*)calloc(num_philosophers, sizeof(int));
  if(block_time == NULL){
    err(1, "Can't allocate memory for block_time.");
  }
  // Array to keep track of when the block time is starting.
  block_starting = (int*)calloc(num_philosophers, sizeof(int));
  if(block_starting == NULL){
    err(1, "Can't allocate memory for block_starting.");
  }
  // Array to keep track of sleeping time/
  thinking_time = (int*)calloc(num_philosophers, sizeof(int));
  if(thinking_time == NULL){
    err(1, "Can't allocate memory for thinking_time.");
  }
  // Array to keep track of the thinking time.
  eating_time = (int*)calloc(num_philosophers, sizeof(int));
  if(eating_time == NULL){
    err(1, "Can't allocate memory for eating_time.");
  }
  memset(block_time, 0, num_philosophers);
  for(int i = 0; i < num_philosophers; i++){
    block_starting[i] = -1;
  }
  memset(thinking_time, 0, num_philosophers);
  memset(eating_time, 0, num_philosophers);
  block_monitor = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(block_monitor, NULL);

  /*
   * Unique to each philosopher.
   */
  for(int i = 0; i < num_philosophers; i++){
    philos[i].id = i;
    philos[i].appetite = max_appetite; //XXX bounded_random(1, max_appetite);
    philos[i].start_time = t0;
    philos[i].max_sleep = max_sleep;
    philos[i].local_vals = philo_local_vals;
    philos[i].total_block_time = block_time;
    philos[i].start_block_time = block_starting;
    philos[i].total_eating_time = eating_time;
    philos[i].total_thinking_time = thinking_time;
    philos[i].state = philo_undefined;
    philos[i].count = num_philosophers;
    philos[i].block_monitor = block_monitor;

    philosopher_print(stdout, &(philos[i]));

    pthread_create(&(threads[i]), NULL, philosopher_thread_entry_point, (void*)(&(philos[i])));
  }

  int num_intervals = run_time / 60;
  int remaining_run_time = run_time % 60;
  assert(num_intervals >= 1);
  debug("Total run time is %d.\n", run_time);
  for(int i = 0; i < num_intervals; i++){
    debug("Threads created and started; main going to sleep for 60 seconds.\n", run_time);
    sleep(60);
    for(int j = 0; j < num_philosophers; j++){
      philosopher_print(stdout, &(philos[j]));
    }
  }
  if(remaining_run_time > 0){
    debug("Main going to sleep for %d seconds.\n", remaining_run_time);
    sleep(remaining_run_time);
  }
  debug("Time is up!\n");
  for(int i = 0; i < num_philosophers; i++){
    debug("Cancelling thread %d...\n", philos[i].id);
    int rv = pthread_cancel(threads[i]);
    if(rv != 0){
      debug("Something went wrong with thread %d", i);
      perror("Thread error");
    }
  }

  // Statistics
  for(int i = 0; i < num_philosophers; i++){
    philosopher_print(stdout, &(philos[i]));
  }
  
  return 0;
}
