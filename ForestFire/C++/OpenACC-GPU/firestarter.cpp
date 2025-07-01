/** firestarter.cpp
 * Based on a solution from David Joiner
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
 * Usage: Fire [numTrials(5000)] * [numProbabilities(101)] [showGraph(1)]
 * The forest size can be set in the #define statement below and in Forest.cuh
 *
 * This OpenACC (GPU) version of the starter code has been modified in the following ways:
 * - The forest_size is now defined at the top of this file instead of being modified through command line arguments
 * - The forest is statically allocated instead of dynamically allocated
 * - The lcrng (random number generator) module is used instead of Tina's RNG.
 *      This is due to easier compatibility with device/GPU code.
 */
#include <stdio.h>
#include <stdlib.h>
#include "X-graph.h"
#include "omp.h"           // OpenMP timing
#include "FireSimulator.h" // class for running the simulation

#define FOREST_SIZE 20

extern void processCommandLine(int argc, char **argv, int &n_trials, int &n_probs, int &do_display);

int main(int argc, char **argv)
{
    // initial conditions and variable definitions
    int forest_size = FOREST_SIZE;
    double prob_min = 0.0;
    double prob_max = 1.0;
    int n_trials = 5000;
    int n_probs = 101;
    int do_display = 1;
    xgraph thegraph;
    xgraph iterationgraph;

    double startTime, totalTime; // timing

    // check command line arguments
    processCommandLine(argc, argv, n_trials, n_probs, do_display);

    // setup problem
    // must be defined using malloc (not "new") in order to be properly sent to the device and later freed.
    double *prob_spread = (double *)calloc(n_probs, sizeof(double));
    double *percent_burned = (double *)calloc(n_probs, sizeof(double));
    double *iterations_spread = (double *)calloc(n_probs, sizeof(double));

    double prob_step = (prob_max-prob_min)/(double)(n_probs-1);
    printf("Probability of fire spreading, Average percent burned, Iterations\n");

    // create a struct to manage the retrieval of random numbers. Initialize it with [numProcesses = 1] and [myRank/ID = 0]
    lcrng randomNumberGenerator = lcrng_initialize(1, 0);

    // the second and third arguments are used for the random number generator. See FireSimulator.h for more details.
    FireSimulator simulator(FOREST_SIZE, randomNumberGenerator);

    startTime = omp_get_wtime(); // start time

    // setup the probabilities that will be used in the simulation
    for (int i_prob = 0; i_prob < n_probs; ++i_prob)
    {
        prob_spread[i_prob] = prob_min + (double)i_prob * prob_step;
    }

    // for a number of trials, calculate average percent burn
    for (int i_trial = 0; i_trial < n_trials; ++i_trial)
    {
        // for a number of probabilities, calculate average burn and output
        for (int i_prob = 0; i_prob < n_probs; ++i_prob)
        {
            // burn until fire is gone
            iterations_spread[i_prob] += simulator.run(prob_spread[i_prob], forest_size / 2, forest_size / 2);
            percent_burned[i_prob] += simulator.getPercentBurned();
        }
    }

    // print the results
    for (int i_prob = 0; i_prob < n_probs; ++i_prob)
    {
        percent_burned[i_prob] /= n_trials;
        iterations_spread[i_prob] /= n_trials;

        // print output
        printf("%lf , %lf, %lf\n", prob_spread[i_prob],
               percent_burned[i_prob],
               iterations_spread[i_prob]);
    }

    // Version of the loop that prints results in real time
    // for (int i_prob = 0; i_prob < n_probs; ++i_prob)
    // {
    //     // for a number of trials, calculate average percent burn
    //     prob_spread[i_prob] = prob_min + (double)i_prob * prob_step;
    //     for (int i_trial = 0; i_trial < n_trials; ++i_trial)
    //     {
    //         // burn until fire is gone
    //         iterations_spread[i_prob] += simulator.run(prob_spread[i_prob], forest_size / 2, forest_size / 2);
    //         percent_burned[i_prob] += simulator.getPercentBurned();
    //     }

    //     iterations_spread[i_prob] /= n_trials;
    //     percent_burned[i_prob] /= n_trials;

    //     // print output
    //     printf("%lf , %lf, %lf\n", prob_spread[i_prob],
    //            percent_burned[i_prob],
    //            iterations_spread[i_prob]);
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
    free(prob_spread);
    free(percent_burned);
    free(iterations_spread);
    return 0;
}

void processCommandLine(int argc, char **argv, int &n_trials, int &n_probs, int &do_display)
{
    if (argc > 1)
    {
        sscanf(argv[1], "%d", &n_trials);
    }
    if (argc > 2)
    {
        sscanf(argv[2], "%d", &n_probs);
    }
    if (argc > 3)
    {
        sscanf(argv[3], "%d", &do_display);
    }
    if (do_display != 0)
        do_display = 1;
}