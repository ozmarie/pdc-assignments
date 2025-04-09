/** FireSimulator.h defines a class for running forest fire simulations.
 * The simulator object contains a forest object
 * and the size of the forest the simulation is being run on.
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
 * Based on firestarter.c by David Joiner
 */

#include "Forest.h"

class FireSimulator
{
public:
    FireSimulator(int forestSize, lcrng RNG)
        : myForestSize(forestSize), myRNG(RNG), myForest(Forest(forestSize)) {}
    int run(double probSpread, int startRow, int startColumn);
    double getPercentBurned();

private:
    int myForestSize;
    lcrng myRNG;
    Forest myForest;
};
