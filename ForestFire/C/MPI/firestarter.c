/** firestarter.c
 * Original Version by David Joiner
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
 * Usage: Fire [forestSize(20)] [numTrials(5000)] * [numProbabilities(101)] [showGraph(1)]
 */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h> // OpenMP timing
#include "X-graph.h"
#include "parallelLoopChunks.h"
#include "lcrng.h"

#define UNBURNT 0
#define SMOLDERING 1
#define BURNING 2
#define BURNT 3

#define true 1
#define false 0

typedef int boolean;

extern void processCommandLine(int argc, char **argv, int *forest_size, int *n_trials, int *n_probs, int *do_display);
extern int **allocate_forest(int);
extern void initialize_forest(int, int **);
extern double get_percent_burned(int, int **);
extern void delete_forest(int, int **);
extern void light_tree(int, int **, int, int);
extern boolean forest_is_burning(int, int **);
extern void forest_burns(int, int **, double, lcrng *);
extern int burn_until_out(int, int **, double, int, int, lcrng *);
extern void print_forest(int, int **);

int main(int argc, char **argv)
{
    // initial conditions and variable definitions
    int forest_size = 20;
    double prob_min = 0.0;
    double prob_max = 1.0;
    int n_trials = 5000;
    int n_probs = 101;
    int do_display = 1;
    xgraph thegraph;
    xgraph iterationgraph;

    double startTime, totalTime; // MPI_Wtime

    // check command line arguments
    processCommandLine(argc, argv, &forest_size, &n_trials, &n_probs, &do_display);

    // setup problem
    // create a struct to manage the retrieval of random numbers. Initialize it with [numProcesses = 1] and [myRank/ID = 0]
    lcrng randomNumberGenerator = lcrng_initialize(1, 0);

    int **forest = allocate_forest(forest_size);
    double *prob_spread = (double *)calloc(n_probs, sizeof(double));
    double *percent_burned = (double *)calloc(n_probs, sizeof(double));
    double *iterations_spread = (double *)calloc(n_probs, sizeof(double));

    // for a number of probabilities, calculate
    // average burn and output
    double prob_step = (prob_max - prob_min) / (double)(n_probs - 1);
    printf("Probability of fire spreading, Average percent burned, Iterations\n");
    startTime = omp_get_wtime(); // start time

    // create an array of probabilities to be simulated
    for (int i_prob = 0; i_prob < n_probs; ++i_prob)
    {
        prob_spread[i_prob] = prob_min + (double)i_prob * prob_step;
    }

    for (int i_trial = 0; i_trial < n_trials; ++i_trial)
    {
        // for a number of trials, calculate average percent burned
        for (int i_prob = 0; i_prob < n_probs; ++i_prob)
        {
            // burn until fire is gone
            iterations_spread[i_prob] += burn_until_out(forest_size, forest, prob_spread[i_prob],
                                                        forest_size / 2, forest_size / 2, &randomNumberGenerator);
            percent_burned[i_prob] += get_percent_burned(forest_size, forest);
        }
    }

    // calculate and print out the results
    for (int i_prob = 0; i_prob < n_probs; ++i_prob)
    {
        iterations_spread[i_prob] /= n_trials;
        percent_burned[i_prob] /= n_trials;

        // print output
        printf("%lf , %lf, %lf\n", prob_spread[i_prob],
               percent_burned[i_prob],
               iterations_spread[i_prob]);
    }

    // Version of the loop that prints results in real time
    // for (i_prob = 0 ; i_prob < n_probs; ++i_prob) {
    //     //for a number of trials, calculate average
    //     // percent burned
    //     prob_spread[i_prob] = prob_min + (double)i_prob * prob_step;
    //     for (i_trial=0; i_trial < n_trials; ++i_trial) {
    //         //burn until fire is gone
    //         iterations_spread[i_prob]+=burn_until_out(forest_size,forest,prob_spread[i_prob],
    //             forest_size/2,forest_size/2, &randomNumberGenerator);
    //         percent_burned[i_prob]+=get_percent_burned(forest_size,forest);
    //     }

    //     iterations_spread[i_prob]/=n_trials;
    //     percent_burned[i_prob]/=n_trials;

    //     // print output
    //     printf("%lf , %lf, %lf\n",prob_spread[i_prob],
    //         percent_burned[i_prob],
    //         iterations_spread[i_prob]);
    // }

    totalTime = omp_get_wtime() - startTime; // end time after processing
    printf("Time taken: %f seconds\n", totalTime);

    // plot graph
    if (do_display == 1)
    {
        // probabilities
        xgraphSetup(&thegraph, 300, 300, "Percent Burned");
        xgraphDraw(&thegraph, n_probs, 0, 0, 1, 1, prob_spread, percent_burned);

        // iterations
        xgraphSetup(&iterationgraph, 300, 300, "Iterations Burning");
        xgraphDraw(&iterationgraph, n_probs, 0, 1, 1, forest_size * 2, prob_spread, iterations_spread);

        // pause the program so that the user can view the displayed graphs.
        printf("Press enter to continue...");
        getchar(); // https://stackoverflow.com/questions/9386651/pause-screen-at-program-completion-in-c
    }

    // clean up
    delete_forest(forest_size, forest);
    free(prob_spread);
    free(percent_burned);
    free(iterations_spread);
    return 0;
}

void processCommandLine(int argc, char **argv, int *forest_size, int *n_trials, int *n_probs, int *do_display)
{
    if (argc > 1)
    {
        sscanf(argv[1], "%d", forest_size);
    }
    if (argc > 2)
    {
        sscanf(argv[2], "%d", n_trials);
    }
    if (argc > 3)
    {
        sscanf(argv[3], "%d", n_probs);
    }
    if (argc > 4)
    {
        sscanf(argv[4], "%d", do_display);
    }
    if (*do_display != 0)
        *do_display = 1;
}

int burn_until_out(int forest_size, int **forest, double prob_spread,
                   int start_i, int start_j, lcrng *rng)
{
    int count;

    initialize_forest(forest_size, forest);
    light_tree(forest_size, forest, start_i, start_j);

    // burn until fire is gone
    count = 0;
    while (forest_is_burning(forest_size, forest))
    {
        forest_burns(forest_size, forest, prob_spread, rng);
        ++count;
    }

    return count;
}

double get_percent_burned(int forest_size, int **forest)
{
    int i, j;
    int total = forest_size * forest_size - 1;
    int sum = 0;

    // calculate pecrent burned
    for (i = 0; i < forest_size; ++i)
    {
        for (j = 0; j < forest_size; ++j)
        {
            if (forest[i][j] == BURNT)
            {
                ++sum;
            }
        }
    }

    // return percent burned;
    return ((double)(sum - 1) / (double)total);
}

int **allocate_forest(int forest_size)
{
    int i;
    int **forest;

    forest = (int **)malloc(sizeof(int *) * forest_size);
    for (i = 0; i < forest_size; ++i)
    {
        forest[i] = (int *)malloc(sizeof(int) * forest_size);
    }

    return forest;
}

void initialize_forest(int forest_size, int **forest)
{
    int i, j;

    for (i = 0; i < forest_size; ++i)
    {
        for (j = 0; j < forest_size; ++j)
        {
            forest[i][j] = UNBURNT;
        }
    }
}

void delete_forest(int forest_size, int **forest)
{
    int i;

    for (i = 0; i < forest_size; ++i)
    {
        free(forest[i]);
    }
    free(forest);
}

void light_tree(int forest_size, int **forest, int i, int j)
{
    forest[i][j] = SMOLDERING;
}

boolean fire_spreads(double prob_spread, lcrng *rng)
{
    int random = lcrng_random(&RNG);
    return (double)random / (double)RAND_MAX < prob_spread;
}

void forest_burns(int forest_size, int **forest, double prob_spread, lcrng *rng)
{
    int i, j;
    extern boolean fire_spreads(double, lcrng *);

    // burning trees burn down, smoldering trees ignite
    for (i = 0; i < forest_size; ++i)
    {
        for (j = 0; j < forest_size; ++j)
        {
            if (forest[i][j] == BURNING)
                forest[i][j] = BURNT;
            else if (forest[i][j] == SMOLDERING)
                forest[i][j] = BURNING;
        }
    }

    // unburnt trees catch fire
    for (i = 0; i < forest_size; ++i)
    {
        for (j = 0; j < forest_size; ++j)
        {
            if (forest[i][j] == BURNING)
            {
                if (i != 0)
                { // North
                    if (fire_spreads(prob_spread, rng) && forest[i - 1][j] == UNBURNT)
                    {
                        forest[i - 1][j] = SMOLDERING;
                    }
                }
                if (i != forest_size - 1)
                { // South
                    if (fire_spreads(prob_spread, rng) && forest[i + 1][j] == UNBURNT)
                    {
                        forest[i + 1][j] = SMOLDERING;
                    }
                }
                if (j != 0)
                { // West
                    if (fire_spreads(prob_spread, rng) && forest[i][j - 1] == UNBURNT)
                    {
                        forest[i][j - 1] = SMOLDERING;
                    }
                }
                if (j != forest_size - 1)
                { // East
                    if (fire_spreads(prob_spread, rng) && forest[i][j + 1] == UNBURNT)
                    {
                        forest[i][j + 1] = SMOLDERING;
                    }
                }
            }
        }
    }
}

boolean forest_is_burning(int forest_size, int **forest)
{
    int i, j;

    for (i = 0; i < forest_size; ++i)
    {
        for (j = 0; j < forest_size; ++j)
        {
            if (forest[i][j] == SMOLDERING || forest[i][j] == BURNING)
            {
                return true;
            }
        }
    }
    return false;
}

void print_forest(int forest_size, int **forest)
{
    int i, j;

    for (i = 0; i < forest_size; ++i)
    {
        for (j = 0; j < forest_size; ++j)
        {
            if (forest[i][j] == BURNT)
            {
                printf(".");
            }
            else
            {
                printf("X");
            }
        }
        printf("\n");
    }
}
