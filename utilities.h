/*
 * CPSC 351-02 Summer 2018 Session B
 * Assignment 3 Dining Philosophers
 * Michael Shafae
 * mshafae at fullerton edu
 *
 * Header file to define a few utilities..
 */

#define PTHREAD_NAME_LENGTH 16

extern char* _program_name;

void debug(const char* format, ...);

long int bounded_random(long int min, long int max);

float frandom( );