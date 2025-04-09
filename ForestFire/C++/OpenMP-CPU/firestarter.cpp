/** firestarter.cpp
 * Based on a solution from David Joiner
 * Usage: Fire [forestSize(20)] [numTrials(5000)] * [numProbabilities(101)] [showGraph(1)]
 */
#include <stdio.h>
#include <stdlib.h>
#include "X-graph.h"
#include "omp.h"           // OpenMP
#include "FireSimulator.h" // class for running the simulation

extern void processCommandLine(int argc, char **argv, int &forest_size, int &n_trials, int &n_probs, int &do_display);

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

    double startTime, totalTime; // timing

    // check command line arguments
    processCommandLine(argc, argv, forest_size, n_trials, n_probs, do_display);

    // setup problem
    double *prob_spread = new double[n_probs];
    double *percent_burned = new double[n_probs];
    double *iterations_spread = new double[n_probs];

    double prob_step = (prob_max-prob_min)/(double)(n_probs-1);
    printf("Probability of fire spreading, Average percent burned, Iterations\n");

    // compute the number of random numbers to reserve for the problem.
    unsigned int numRandoms = n_probs * n_trials * forest_size * 4; // (n_probs*n_trials) * possible trees that could burn * number of directions to spread fire
    int startRandoms = 0 * numRandoms;                              // Where this process should start getting random numbers from (0 because this is sequential)
    yarn2 RandomNumberEngine;
    uniform01_dist<> dist;
    RandomNumberEngine.jump((unsigned long long)startRandoms);

    // the second and third arguments are used for the random number generator. See FireSimulator.h for more details.
    FireSimulator simulator(forest_size, RandomNumberEngine, dist);

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
    delete[] prob_spread;
    delete[] percent_burned;
    delete[] iterations_spread;
    return 0;
}

void processCommandLine(int argc, char **argv, int &forest_size, int &n_trials, int &n_probs, int &do_display)
{
    if (argc > 1)
    {
        sscanf(argv[1], "%d", &forest_size);
    }
    if (argc > 2)
    {
        sscanf(argv[2], "%d", &n_trials);
    }
    if (argc > 3)
    {
        sscanf(argv[3], "%d", &n_probs);
    }
    if (argc > 4)
    {
        sscanf(argv[4], "%d", &do_display);
    }
    if (do_display != 0) do_display = 1;
}
