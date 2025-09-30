''' FireSimulator.py
Defines a class for running forest fire simulations
The simulator object contains a forest object and
the size of the forest the simulation is being run on.

Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
Based on firestarter.c by David Joiner
'''
from Forest import Forest

class FireSimulator:
    def __init__(self, forestSize):
        self.forestSize = forestSize
        self.forest = Forest(forestSize)

    ''' run()
    runs one trial of the fire simulation.
    Returns the number of iterations the simulation ran for.
    '''
    def run(self, probSpread, startRow, startColumn):
        self.forest.reset()
        self.forest.forest[startRow][startColumn].light()

        count = 0
        while (self.forest.forestIsBurning()):
            # run iteration
            for i in range(self.forestSize):
                for j in range(self.forestSize):
                    self.forest.forest[i][j].continueBurning()
            for i in range(self.forestSize):
                for j in range(self.forestSize):
                    self.forest.treeBurns(i, j, probSpread)
            count += 1
        return count # iterations spent burning

    def getPercentBurned(self):
        return self.forest.getPercentBurned()