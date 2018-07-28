/*
 * CPSC 351-02 Summer 2018 Session B
 * Assignment 3 Dining Philosophers
 * Michael Shafae
 * mshafae at fullerton edu
 *
 * Handy utilities for the project
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "utilities.h"

char* _program_name;

/** Convert a timespec struct to a nicely formatted string.
 * This function was taken from
 * https://stackoverflow.com/questions/8304259/formatting-struct-timespec.
 * The author is Thomas Tempelmann.
 */
int timespec2str(char *buffer, unsigned int length, struct timespec *ts) {
  int rv;
  struct tm t;
  tzset( );
  if(localtime_r(&(ts->tv_sec), &t) == NULL){
    return 1;
  }
  rv = strftime(buffer, length, "%T", &t);
  if(rv == 0){
    return 2;
  }
  length -= rv- 1;
  rv = snprintf(&buffer[strlen(buffer)], length, ".%09ld", ts->tv_nsec);
  if (rv >= length){
    return 3;
  }
  return 0;
}

/** Pass through function to aide in debugging.
 * The function doesn't do much other than act like a wrapper
 * around fprintf( ). It's handy because it's clearly meant for
 * debugging.
 */
void debug(const char* format, ...){
  va_list args;
  char buffer[512];
  char thread_id_s[PTHREAD_NAME_LENGTH];
  clockid_t clk_id = CLOCK_REALTIME;
  char timestr[30];
  struct timespec ts, res;
  pid_t pid = 0;
  pthread_t thread_id = 0;
  pid = getpid( );
  thread_id = pthread_self( );
  pthread_getname_np(thread_id, thread_id_s, PTHREAD_NAME_LENGTH);
  clock_getres(clk_id, &res);
  clock_gettime(clk_id, &ts);
  va_start(args, format);
  vsnprintf(buffer, 512, format, args);
  va_end(args);
  if(timespec2str(timestr, sizeof(timestr), &ts) != 0){
    fprintf(stderr, "timespec2str failed\n");
    exit(1);
  }
  fprintf(stderr, "%s(%d-%s): %s: %s", _program_name, pid, thread_id_s, timestr, buffer);
}

/** A random number between min and max.
 * Using random(3), generate a random number that is between min and
 * max. The function assumes that the generator has been previously
 * seeded and initialized.
 */
long int bounded_random(long int min, long int max){
  return min + (random( ) % max);
}

/** A random number between 0 and 1.0.
 * Using random(3), generate a random number and divide it by
 * RAND_MAX thus returning a value between 0 and 1.
 */
float frandom( ){
  float r = (float)random( );
  return r / ((float)RAND_MAX);
}
