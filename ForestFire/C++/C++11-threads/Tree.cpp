/** Tree.cpp defines a class for individual "Tree" objects
 * These Trees contain information about their individual state
 *
 * Tree methods add support for retriving and modifying
 * the "state" of the Tree.
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
 * Based on firestarter.c by David Joiner
 */
#include "Tree.h"

/** default constructor
 * POSTCONDITION: state == UNBURNT
 */
Tree::Tree()
{
    myState = UNBURNT;
}

/** copy constructor
 * @param original: a Tree
 * POSTCONDITION: I am a distinct copy of original.
 */
Tree::Tree(const Tree &original)
{
    myState = original.myState;
}

int Tree::getState()
{
    return myState;
}

void Tree::setState(int newState)
{
    myState = newState;
}

// Assign the value of a new tree to this tree
void Tree::operator=(Tree newTree)
{
    myState = newTree.myState;
}

// Assign a new state to this tree's state
void Tree::operator=(int newState)
{
    myState = newState;
}

// is this tree's state equal to the given state?
bool Tree::operator==(int state)
{
    return myState == state;
}

/** light()
 * Lights a tree on fire (sets it to smoldering)
 */
void Tree::light()
{
    setState(SMOLDERING);
}

/** continueBurning()
 * used to determine whether a tree's state should be
 * changed to "BURNING" or "BURNT".
 */
void Tree::continueBurning()
{
    // a burning tree burns down, a smoldering tree ignites
    if (getState() == BURNING)
        setState(BURNT);
    else if (getState() == SMOLDERING)
        setState(BURNING);
}