''' firestarter.py
Based on a solution from David Joiner
Usage: Fire [forestSize(20)] [numTrials(5000)] * [numProbabilities(101)] [showGraph(1)]
 '''

import sys # command line arguments
import time # timing functions
from FireSimulator import FireSimulator
import matplotlib.pyplot as plt
import math
# from mpi4py import MPI
import numpy as np

def main():
    forest_size = 20
    prob_min = 0.0
    prob_max = 1.0
    n_trials = 5000
    n_probs = 101
    do_display = 1

    # process command line arguments
    if (len(sys.argv) > 1):
        forest_size = int(sys.argv[1])
    if (len(sys.argv) > 2):
        n_trials = int(sys.argv[2])
    if (len(sys.argv) > 3):
        n_probs = int(sys.argv[3])
    if (len(sys.argv) > 4):
        do_display = int(sys.argv[4])

    # NOTE: comm.Reduce was not working with python lists, so numpy arrays are used instead.
    prob_spread = np.zeros(n_probs)
    percent_burned = np.zeros(n_probs)
    iterations_spread = np.zeros(n_probs)

    prob_step = (prob_max-prob_min) / (n_probs - 1)
    print("Probability of fire spreading, Average percent burned, Iterations\n")
    startTime = time.time()

    simulator = FireSimulator(forest_size)

    for i_prob in range(n_probs):
        prob_spread[i_prob] = prob_min + i_prob * prob_step

    # for a number of trials, calculate average percent burn
    for i_trial in range(n_trials):
        # Version of the loop that prints results in real time
        for i_prob in range(n_probs):
            # burn until fire is gone
            iterations_spread[i_prob] += simulator.run(prob_spread[i_prob], forest_size // 2, forest_size // 2)
            percent_burned[i_prob] += simulator.getPercentBurned()

    for i_prob in range(n_probs):
        percent_burned[i_prob] /= n_trials
        iterations_spread[i_prob] /= n_trials

        # print output
        print(f"{prob_spread[i_prob]:.2f}, {percent_burned[i_prob]}, {iterations_spread[i_prob]}\n")

    # # Version of the loop that prints results in real time
    # for i_prob in range(n_probs):
    #     # for a number of trials, calculate average percent burn
    #     prob_spread[i_prob] = prob_min + i_prob * prob_step
    #     for i_trial in range(n_trials):
    #         # burn until fire is gone
    #         iterations_spread[i_prob] += simulator.run(prob_spread[i_prob], forest_size // 2, forest_size // 2)
    #         percent_burned[i_prob] += simulator.getPercentBurned()

    #     iterations_spread[i_prob] /= n_trials
    #     percent_burned[i_prob] /= n_trials

    #     # print output
    #     print(f"{prob_spread[i_prob]:.2f}, {percent_burned[i_prob]}, {iterations_spread[i_prob]}\n")

    totalTime = time.time() - startTime
    print(f"Time taken: {totalTime} seconds\n")

    # plot graph
    if do_display == 1:
        # probabilities
        plt.plot(prob_spread, percent_burned)
        plt.xlabel('Probability of Fire Spreading')
        plt.ylabel('Percent Burned')
        plt.title('Percent Burned by Probability')

        plt.show()

        # iterations
        plt.plot(prob_spread, iterations_spread)
        plt.xlabel('Probability of Fire Spreading')
        plt.ylabel('Iterations Burning')
        plt.title('Iterations Burning by Probability')

        plt.show()

def getChunkStartStopValues(id, numProcs, REPS):
    if numProcs > REPS:
        if id == 0:
            print(f"\n*** Number of processes/threads (%u) exceeds REPS (%u)\n", numProcs, REPS)
            print(f"*** Please run with -np (number of processes/threads) less than or equal to %u\n\n", REPS)
        MPI.Finalize()
        exit(1)
    
    # compute the chunk size that works in many cases
    chunkSize1 = math.ceil(REPS / numProcs)
    begin = id * chunkSize1
    end = begin + chunkSize1
    # see if there are any leftover iterations
    remainder = REPS % numProcs
    # If remainder == 0, chunkSize1 = chunk-size for all processes/threads;
    # If remainder != 0, chunkSize1 = chunk-size for p_0..p_remainder-1
    #   but for processes/threads p_remainder..p_numProcs-1
    #   recompute begin and end using a smaller-by-1 chunk size, chunkSize2.
    if remainder > 0 and id >= remainder:
        chunkSize2 = chunkSize1 - 1
        remainderBase = remainder * chunkSize1
        processOffset = (id - remainder) * chunkSize2
        begin = remainderBase + processOffset
        end = begin + chunkSize2
        
    return begin, end

main()