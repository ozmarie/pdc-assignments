/** Forest.h defines a class for "Forest" objects,
 * Which are a collection of "Trees".
 * Beyond containing a collection of trees, the Forest
 * also contains information about its size.
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
 * Based on firestarter.c by David Joiner
 */

#include <stdio.h>
#include <stdlib.h>
#include "X-graph.h"
#include "omp.h" // OpenMP timing and gpu offloading
#include "Tree.h"
#include "lcrng.h" // random number generator

#define UNBURNT 0
#define SMOLDERING 1
#define BURNING 2
#define BURNT 3

#define forestDim 20

class Forest
{
public:
    Forest();
    Forest(int forestSize) : myForestSize(forestSize) {}
    Forest(const Forest &original);
    bool forestIsBurning();
    void print();
    double getPercentBurned();
    void reset();
    void treeBurns(int row, int column, double probSpread, lcrng &RNG);
    const Tree *operator[](unsigned index) const;
    Tree *operator[](unsigned index);

private:
    Tree myForest[forestDim][forestDim];            // an array of trees
    int myForestSize;           // the length of one side of a forest (of size forestSize x forestSize)
    friend class FireSimulator; // used to allow FireSimulator to access myForest
};

// default constructor
inline Forest::Forest()
{
    myForestSize = 0;
}

/** copy constructor
 * Parameters: original, a Forest
 * Postcondition: I am a distinct copy of original.
 */
inline Forest::Forest(const Forest &original)
{
    myForestSize = original.myForestSize;
}

// allows for the syntax Forest[i][j], but Forest.myForest[i][j] is more efficient
inline const Tree *Forest::operator[](unsigned index) const
{
    return myForest[index];
}

inline Tree *Forest::operator[](unsigned index)
{
    return myForest[index];
}