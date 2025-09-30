/** FireSimulator.cpp defines a class for running forest fire simulations.
 * The simulator object contains a forest object
 * and the size of the forest the simulation is being run on.
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
 * Based on firestarter.c by David Joiner
 */

#include "FireSimulator.h"

#pragma omp declare target(FireSimulator::run, FireSimulator::getPercentBurned)

/** run()
 * runs one trial of the fire simulation.
 * Returns the number of iterations the simulation ran for.
 */
int FireSimulator::run(double probSpread, int startRow, int startColumn)
{

    myForest.reset();
    myForest.myForest[startRow][startColumn].light();

    int count = 0;
    while (myForest.forestIsBurning())
    {
        // run iteration
        for (int i = 0; i < myForestSize; ++i)
        {
            for (int j = 0; j < myForestSize; ++j)
            {
                myForest.myForest[i][j].continueBurning();
            }
        }
        for (int i = 0; i < myForestSize; ++i)
        {
            for (int j = 0; j < myForestSize; ++j)
            {
                myForest.treeBurns(i, j, probSpread, myRNG);
            }
        }
        ++count;
    }

    return count;
}

double FireSimulator::getPercentBurned()
{
    return myForest.getPercentBurned();
}