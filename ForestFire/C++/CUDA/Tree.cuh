/** Tree.cuh declares a class for individual "Tree" objects
 * These Trees contain information about their individual state
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
 * Based on firestarter.c by David Joiner
 */

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdexcept>

#define UNBURNT 0
#define SMOLDERING 1
#define BURNING 2
#define BURNT 3

using namespace std;

class Tree
{
public:
    Tree();
    Tree(const Tree &original);
    int getState();
    void setState(int newState);
    void light();
    void continueBurning();
    void operator=(Tree newTree);
    void operator=(int newState);
    bool operator==(int state);

private:
    int myState;
};