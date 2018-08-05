/*
 * CPSC 351-02 Summer 2018 Session B
 * Assignment 3 Dining Philosophers
 * Michael Shafae
 * mshafae at fullerton edu
 *
 * Header file to define the interface to the Philosopher module.
 */

#include <pthread.h>
#include <stdio.h>

#define num_philo 4

typedef enum{
  philo_thinking = 0,
  philo_hungry = 1,
  philo_eating = 2,
  philo_undefined = 3,
}philostate;

extern const char* philostate_str[];

// struct for local values
struct data{
	int philo_status[num_philo];
}localdata;

typedef struct Philosopher{
  /* Philosopher's ID; be aware that thread_t is an opaque type and you should
   * use pthread_getname_np( ) or this id to identify the thread & philosopher
   */
  int id;
  /* The Philosopher's appetite; the higher the value, the more the
   * the philosopher wants to eat. It is also the number of seconds that the
   * philosopher wants to spend eating.
   */
  int appetite;
  /* The time in seconds of when the program/thread started. */
  long start_time;
  /* The maximum time in seconds that the philosopher is sleeps or eats */
  long max_sleep;
  /* A void* (void pointer) to a struct that you may (should!) define.
   * Initially, consider that this pointer can point to a struct that contains
   * mutexes for the chopsticks and that this struct is shared between all
   * philosophers.
   *
   * You may define this struct in this file or in an additional header file.
   * If you define it in an additional header file then make sure the Makefile
   * has been updated and the file is added to your repository.
   */
  void *local_vals;
  /* The total time in seconds that a philosopher is blocked. */
  int *total_block_time;
  /* If the philosopher is currently blocked, this is the time in seconds
   * of when the block started.
   */
  int *start_block_time;
  /* Add up all the time the philosopher spends eating. */
  int *total_eating_time;
  /* Add up all the time spent thinking. */
  int *total_thinking_time;
  /* What the philosopher is currently doing. Use the enumerated type 
   * philostate defined above to set the state for this field.
   */
  philostate state;
  /* The count of the number of philosophers. */
  int count;
  /* Monitor for the philosopher's block time. */
  pthread_mutex_t *block_monitor;
}Philosopher;


/** Allocate, initialize and return the pointer to the local_vals struct
 * as a void*. Be aware that to make sense of the struct, you'll have to cast
 * the pointer to an appropriate type.
 *
 * You may define this struct in this file or in an additional header file. If
 * you define it in an additional header file then make sure the Makefile has
 * been updated and the file is added to your repository.
 */
void* philosopher_init_local_vals(int philo_count);

/** The philosopher passed as a parameter attempts to pick up a chopstick.
 * Whether it is the 'left', 'right', or both chopsticks is up to you.
 */
void philosopher_chopstick_pickup(Philosopher *philo);

/** The philosopher passed as a paramter attempts to putdown a chopstick.
 * Whether it is the 'left', 'right', or both chopsticks is up to you.
 */
void philosopher_chopstick_putdown(Philosopher *philo);

/** Pretty print a philosopher struct.
 * Print out to stream the contents of the Philosopher struct philo.
 */
int philosopher_print(FILE* stream, Philosopher *philo);





