''' Tree.py
Defines a class for individual "Tree" objects.
These Trees contian information about their individual state.

Tree methods add support for retrieving and modifying 
the "state" of the Tree.

Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
Based on firestarter.c by David Joiner
'''

UNBURNT = 0
SMOLDERING = 1
BURNING = 2
BURNT = 3

class Tree:
    def __init__(self):
        self.state = UNBURNT

    def getState(self):
        return self.state

    def setState(self, newState):
        self.state = newState

    def light(self):
        self.setState(SMOLDERING)

    def continueBurning(self):
        if self.getState() == BURNING:
            self.setState(BURNT)
        elif self.getState() == SMOLDERING:
            self.setState(BURNING)