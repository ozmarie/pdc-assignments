/** Forest.cpp defines a class for "Forest" objects,
 * Which are a collection of "Trees".
 * Beyond containing a collection of trees, the Forest
 * also contains information about its size.
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
 * Based on firestarter.c by David Joiner
 */
#include "Forest.h"

/** getPercentBurned()
 * Returns the percentage of this forest's
 * trees that have been burnt down.
 */
double Forest::getPercentBurned()
{
    int i, j;
    // -1 to exclude the tree that started the fire
    double total = myForestSize * myForestSize - 1;
    double sum = 0;

    // calculate pecrent burned
    for (i = 0; i < myForestSize; ++i)
    {
        for (j = 0; j < myForestSize; ++j)
        {
            if (myForest[i][j].getState() == BURNT)
            {
                ++sum;
            }
        }
    }

    // return percent burned;
    return ((sum - 1) / total);
}

/** forestIsBurning()
 * Returns true if any part of the forest
 * is still smoldering/burning
 */
bool Forest::forestIsBurning()
{
    int i, j;

    for (i = 0; i < myForestSize; ++i)
    {
        for (j = 0; j < myForestSize; ++j)
        {
            if (myForest[i][j].getState() == SMOLDERING || myForest[i][j].getState() == BURNING)
            {
                return true;
            }
        }
    }
    return false;
}

/** print()
 * Prints this forest to the command line,
 * representing unburnt trees as "X",
 * burnt trees as ".",
 */
void Forest::print()
{
    int i, j;
    for (i = 0; i < myForestSize; ++i)
    {
        for (j = 0; j < myForestSize; ++j)
        {
            if (myForest[i][j].getState() == BURNT)
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

/** fire_spreads()
 * Based on a random number and the probability of fire spreading,
 * determines whether a fire should spread or not.
 */
bool fire_spreads(double probSpread, yarn2 &RNE, uniform01_dist<> dist)
{
    // if the generated random number is < the probability of spreading, return true
    return dist(RNE) < probSpread;
}

/** treeBurns()
 * Burn for one iteration to see how this tree's state should change.
 */
void Forest::treeBurns(int row, int column, double probSpread, yarn2 &RNE, uniform01_dist<> dist)
{
    if (myForestSize == 0)
    {
        throw invalid_argument("Forest::treeBurns: This forest has not been initialized");
    }
    extern bool fire_spreads(double, yarn2 &, uniform01_dist<>);

    // check nearby trees to see if I should set them on fire
    if (myForest[row][column].getState() == BURNING)
    {
        if (row != 0)
        { // North
            if (fire_spreads(probSpread, RNE, dist) && myForest[row - 1][column].getState() == UNBURNT)
            {
                myForest[row - 1][column].setState(SMOLDERING);
            }
        }
        if (row != myForestSize - 1)
        { // South
            if (fire_spreads(probSpread, RNE, dist) && myForest[row + 1][column].getState() == UNBURNT)
            {
                myForest[row + 1][column].setState(SMOLDERING);
            }
        }
        if (column != 0)
        { // West
            if (fire_spreads(probSpread, RNE, dist) && myForest[row][column - 1].getState() == UNBURNT)
            {
                myForest[row][column - 1].setState(SMOLDERING);
            }
        }
        if (column != myForestSize - 1)
        { // East
            if (fire_spreads(probSpread, RNE, dist) && myForest[row][column + 1].getState() == UNBURNT)
            {
                myForest[row][column + 1].setState(SMOLDERING);
            }
        }
    }
}

/** reset()
 * sets the state of every tree in
 * the forest to "unburnt".
 */
void Forest::reset()
{
    for (int i = 0; i < myForestSize; ++i)
    {
        for (int j = 0; j < myForestSize; ++j)
        {
            myForest[i][j].setState(UNBURNT);
        }
    }
}