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
// Tina's RNG
#include <trng/yarn2.hpp>
#include <trng/uniform01_dist.hpp>
#include "X-graph.h"
#include "omp.h" // OpenMP timing
#include "Tree.h"

using namespace trng;

#define UNBURNT 0
#define SMOLDERING 1
#define BURNING 2
#define BURNT 3

class Forest
{
public:
    Forest();
    Forest(int forestSize);
    Forest(const Forest &original);
    ~Forest();
    bool forestIsBurning();
    void print();
    double getPercentBurned();
    void reset();
    void treeBurns(int row, int column, double probSpread, yarn2 &RNE, uniform01_dist<> dist);
    const Tree *operator[](unsigned index) const;
    Tree *operator[](unsigned index);

private:
    Tree **myForest;            // an array of trees
    int myForestSize;           // the length of one side of a forest (of size forestSize x forestSize)
    friend class FireSimulator; // used to allow FireSimulator to access myForest
};

// default constructor
inline Forest::Forest()
{
    myForest = nullptr;
    myForestSize = 0;
}

/** constructor
 * initializes a forest of size forestSize * forestSize
 * to be full of unburnt Trees.
 */
inline Forest::Forest(int forestSize)
{
    myForestSize = forestSize;
    myForest = new Tree *[forestSize * forestSize];
    for (int i = 0; i < forestSize; ++i)
    {
        myForest[i] = new Tree[forestSize];
        for (int j = 0; j < forestSize; ++j)
        {
            myForest[i][j] = Tree();
        }
    }
}

/** copy constructor
 * Parameters: original, a Forest
 * Postcondition: I am a distinct copy of original.
 */
inline Forest::Forest(const Forest &original)
{
    myForest = original.myForest;
    myForestSize = original.myForestSize;
}

/** destructor
 * frees memory from myForest
 */
inline Forest::~Forest()
{
    delete[] myForest;
    myForest = nullptr;
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