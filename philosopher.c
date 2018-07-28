/*
 * CPSC 351-02 Summer 2018 Session B
 * Assignment 3 Dining Philosophers
 * Michael Shafae
 * mshafae at fullerton edu
 *
 * Implementation file to the Philosopher module.
 */

#include <stdlib.h>

#include "philosopher.h"

/*
 * The implementation below does nothing. Replace the contents of these
 * functions with a (correct) implementation of the the dining philsophers 
 * solution.
 */

void* philosopher_init_local_vals(int philo_count){
  return NULL;
}

void philosopher_chopstick_pickup(Philosopher *philo){
  return;
}

void philosopher_chopstick_putdown(Philosopher *philo){
  return;
}

/*
 * There should be no reason to change anything below this
 * point, however you are free to do so.
 */

const char* philostate_str[] = {"thinking", "hungry", "eating", "undefined", NULL};

int philosopher_print(FILE* stream, Philosopher *philo){
  int rv;
  rv = fprintf(stream, "Philosopher %d\n", philo->id);
  rv += fprintf(stream, "\tappetite: %d\n", philo->appetite);
  rv += fprintf(stream, "\tstart time: %ld\n", philo->start_time);
  rv += fprintf(stream, "\tmax. sleep time: %ld\n", philo->max_sleep);
  rv += fprintf(stream, "\tlocal vals. addr.: %p\n", philo->local_vals);
  rv += fprintf(stream, "\ttotal block time: %d\n", philo->total_block_time[philo->id]);
  rv += fprintf(stream, "\tstart block time: %d\n", philo->start_block_time[philo->id]);
  rv += fprintf(stream, "\ttotal eating time: %d\n", philo->total_eating_time[philo->id]);
  rv += fprintf(stream, "\ttotal thinking time: %d\n", philo->total_thinking_time[philo->id]);
  rv += fprintf(stream, "\tstate: %s\n", philostate_str[philo->state]);
  rv += fprintf(stream, "\tcount: %d\n", philo->count);
  return(rv);
}

