''' Forest.py
Defines a class for "Forest" objects,
which are a collection of "Trees".
Beyond containing a collection of trees, the Forest
also contains information about its size.

Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
Based on firestarter.c by David Joiner
'''

import random
from Tree import Tree

UNBURNT = 0
SMOLDERING = 1
BURNING = 2
BURNT = 3

class Forest:
    def __init__(self, forestSize):
        # create a forest and initialize all the values to a default tree (all UNBURNT)
        self.forest = [[Tree() for col in range(forestSize)] for row in range(forestSize)]
        self.forestSize = forestSize

    def forestIsBurning(self):
        for i in range(self.forestSize):
            for j in range(self.forestSize):
                if (self.forest[i][j].getState() == SMOLDERING or self.forest[i][j].getState() == BURNING):
                    return True
        return False

    ''' print()
    Prints this forest to the command line,
    representing unburnt trees as "X",
    burnt trees as "."
    '''
    def print(self):
        for i in range(self.forestSize):
            for j in range(self.forestSize):
                if (self.forest[i][j].getState() == BURNT):
                    print(".")
                else:
                    print("X")
            print("\n")

    def getPercentBurned(self):
        # -1 to exclude the tree that started the fire
        total = self.forestSize * self.forestSize - 1
        sum = 0

        for i in range(self.forestSize):
            for j in range(self.forestSize):
                if self.forest[i][j].getState() == BURNT:
                    sum += 1
        return (sum - 1) / total


    def reset(self):
        for i in range(self.forestSize):
            for j in range(self.forestSize):
                self.forest[i][j].setState(UNBURNT)  

    def fireSpreads(self, probSpread):
        return random.random() < probSpread

    def treeBurns(self, row, column, probSpread):
        if self.forestSize == 0:
            # TODO: error handling
            return

        if self.forest[row][column].getState() == BURNING:
            if row != 0:
                if self.fireSpreads(probSpread) and self.forest[row - 1][column].getState() == UNBURNT:
                    self.forest[row - 1][column].setState(SMOLDERING)
            if row != self.forestSize - 1:
                if self.fireSpreads(probSpread) and self.forest[row + 1][column].getState() == UNBURNT:
                    self.forest[row + 1][column].setState(SMOLDERING)
            if column != 0:
                if self.fireSpreads(probSpread) and self.forest[row][column - 1].getState() == UNBURNT:
                    self.forest[row][column - 1].setState(SMOLDERING)
            if column != self.forestSize - 1:
                if self.fireSpreads(probSpread) and self.forest[row][column + 1].getState() == UNBURNT:
                    self.forest[row][column + 1].setState(SMOLDERING)
