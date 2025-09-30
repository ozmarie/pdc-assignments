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
    FireSimulator(int forestSize, yarn2 &RNEngine, uniform01_dist<> distribution)
        : myForestSize(forestSize), myEngine(RNEngine), myDistribution(distribution), myForest(Forest(forestSize)) {}
    int run(double probSpread, int startRow, int startColumn);
    double getPercentBurned();

private:
    int myForestSize;
    yarn2 myEngine;
    uniform01_dist<> myDistribution;
    Forest myForest;
};
