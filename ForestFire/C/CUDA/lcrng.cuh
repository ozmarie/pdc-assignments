/******************************************************************************/
/*
  LCRNG
  Purpose:

    To use many processes and generate the same sequence of random numbers
    that 1 process would.

  Algorithm:

    Linear Congruential Random Number Generator

  Licensing:

    This code is distributed under the GNU LGPL license.

  Author:

    John Burkardt

  From:

    https://people.math.sc.edu/Burkardt/c_src/random_mpi/random_mpi.html

******************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// LCRNG struct and related functions added by Steven McKelvey, Calvin University, August 2024
typedef struct lcrng
{
  int a, b, c, an, bn;
  int currentRandom;
} lcrng;

lcrng lcrng_initialize(int numProcs, int id);
int lcrng_random(lcrng *rng);

int congruence(int a, int b, int c, int *error);
int i4_gcd(int i, int j);
int i4_max(int i1, int i2);
int i4_min(int i1, int i2);
int i4_sign(int i);
void lcrng_anbn(int a, int b, int c, int n, int *an, int *bn);
int lcrng_evaluate(int a, int b, int c, int x);
int power_mod(int a, int n, int m);
void timestamp();