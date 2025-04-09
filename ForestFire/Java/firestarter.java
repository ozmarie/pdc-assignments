/** firestarter.java
 * Based on a C solution from David Joiner
 * compile: mpijavac firestarter.java FireSimulator.java Forest.java Tree.java Graph.java
 * run: mpirun -np <num_processes> -hostfile <hostfile_name> java firestarter [forestSize(20)] [numTrials(5000)] * [numProbabilities(101)] [showGraph(1)]
 */

import java.util.*;
import java.lang.Math; // Math.ceil()
import javax.swing.*; // for graphs (JFrame)
// import mpi.*;

public class firestarter {
    public static void main(String[] args) {
        // initial conditions and variable definitions
        int forest_size = 20;
        double prob_min = 0.0;
        double prob_max = 1.0;
        int n_trials = 5000;
        int n_probs = 101;
        int do_display = 1;

        // process command line arguments
        if (args.length > 0) {
            forest_size = Integer.parseInt(args[0]);
        }
        if (args.length > 1) {
            n_trials = Integer.parseInt(args[1]);
        }
        if (args.length > 2) {
            n_probs = Integer.parseInt(args[2]);
        }
        if (args.length > 3) {
            do_display = Integer.parseInt(args[3]);
        }

        // setup problem
        double[] prob_spread = new double[n_probs];
        double[] percent_burned = new double[n_probs];
        double[] iterations_spread = new double[n_probs];

        double prob_step = (prob_max-prob_min) / (n_probs - 1);
        System.out.println("Probability of fire spreading, Average percent burned, Iterations");

        FireSimulator simulator = new FireSimulator(forest_size);

        double startTime = System.currentTimeMillis();

        for (int i_prob = 0; i_prob < n_probs; ++i_prob) {
            prob_spread[i_prob] = prob_min + i_prob * prob_step;
        }

        // for a number of trials, calculate the average percent burn
        for (int i_trial = 0; i_trial < n_trials; ++i_trial) 
        {
            // for a number of probabilities, calculate the average percent burn
            for (int i_prob = 0; i_prob < n_probs; ++i_prob) 
            {
                iterations_spread[i_prob] += simulator.run(prob_spread[i_prob], Math.floorDiv(forest_size, 2), Math.floorDiv(forest_size, 2));
                percent_burned[i_prob] += simulator.getPercentBurned();
            }  
        }

        for (int i_prob = 0; i_prob < n_probs; ++i_prob) 
        { 
            iterations_spread[i_prob] /= n_trials;
            percent_burned[i_prob] /= n_trials;

            // print the results
            System.out.println(String.format("%f, %f, %f", prob_spread[i_prob], 
                                percent_burned[i_prob], iterations_spread[i_prob]));  
        }

        // Version of the loop that prints results in real time
        // for a number of probabilities, calculate the average percent burn
        // for (int i_prob = 0; i_prob < n_probs; ++i_prob) {
        //     prob_spread[i_prob] = prob_min + i_prob * prob_step;
        //     // for a number of trials, calculate the average percent burn
        //     for (int i_trial = 0; i_trial < n_trials; ++i_trial) {
        //         iterations_spread[i_prob] += simulator.run(prob_spread[i_prob], Math.floorDiv(forest_size, 2), Math.floorDiv(forest_size, 2));
        //         percent_burned[i_prob] += simulator.getPercentBurned();
        //     }

        //     iterations_spread[i_prob] /= n_trials;
        //     percent_burned[i_prob] /= n_trials;

        //     // print the results
        //     System.out.println(String.format("%f, %f, %f", prob_spread[i_prob], 
        //                         percent_burned[i_prob], iterations_spread[i_prob]));    
        // }

        double totalTime = System.currentTimeMillis() - startTime;
        System.out.println(String.format("Time taken: %f seconds", totalTime / 1000.0));

        // plot graph
        JFrame percentFrame = new JFrame("Percent Burned");
        percentFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        percentFrame.add(new Graph(prob_spread, percent_burned));
        percentFrame.setSize(400, 400);
        percentFrame.setLocation(200, 200);
        percentFrame.setVisible(true);

        JFrame iterationsFrame = new JFrame("Iterations Spread");
        iterationsFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        iterationsFrame.add(new Graph(prob_spread, iterations_spread));
        iterationsFrame.setSize(400, 400);
        iterationsFrame.setLocation(200, 200);
        iterationsFrame.setVisible(true);
    }

    public static int[] getChunkStartStopValues(int id, int numProcs, int REPS) {
        if (numProcs > REPS)
        {
            if (id == 0)
            {
                System.err.println(String.format("\n*** Number of processes/threads (%u) exceeds REPS (%u)", numProcs, REPS));
                System.err.println(String.format("*** Please run with -np (number of processes/threads) less than or equal to %u\n", REPS));
            }
            // MPI.Finalize();
            System.exit(1);
        }

        // compute the chunk size that works in many cases
        int chunkSize1 = (int)Math.ceil((double)REPS / numProcs);
        int begin = id * chunkSize1;
        int end = begin + chunkSize1;
        // see if there are any leftover iterations
        int remainder = REPS % numProcs;
        // If remainder == 0, chunkSize1 = chunk-size for all processes/threads;
        // If remainder != 0, chunkSize1 = chunk-size for p_0..p_remainder-1
        //   but for processes/threads p_remainder..p_numProcs-1
        //   recompute begin and end using a smaller-by-1 chunk size, chunkSize2.
        if (remainder > 0 && id >= remainder)
        {
            int chunkSize2 = chunkSize1 - 1;
            int remainderBase = remainder * chunkSize1;
            int processOffset = (id - remainder) * chunkSize2;
            begin = remainderBase + processOffset;
            end = begin + chunkSize2;
        }
        // pass back this proc's/thread's begin and end values via start and stop
        int returnArray[] = {begin, end};
        return returnArray;
    }
}