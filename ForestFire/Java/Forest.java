/** Forest.java
 * Defines a class for "Forest" objects,
 * which are a collection of "Trees".
 * Beyond containing a collection of trees, the Forest
 * also contains information about its size.
 * 
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
 * Based on firestarter.c by David Joiner
 */

import java.util.*;

public class Forest {
    private static final int UNBURNT = 0;
    private static final int SMOLDERING = 1;
    private static final int BURNING = 2;
    private static final int BURNT = 3;

    public Tree[][] forest;
    private int forestSize;

    // default constructor
    public Forest() {
        this.forest = null;
        this.forestSize = 0;
    }

    /** constructor
     * initializes a forest of size forestSize * forestSize
     * to be full of unburnt Trees.
     */
    public Forest(int forestSize) {
        this.forestSize = forestSize;
        this.forest = new Tree[forestSize][forestSize];
        assert this.forest != null;

        // Initialize elements of forest
        for (int i = 0; i < forestSize; ++i) {
            for (int j = 0; j < forestSize; ++j) {
                this.forest[i][j] = new Tree();
            }
        }
    }

    /** copy constructor
     * Parameters: original, a Forest
     * Postcondition: I am a distinct copy of original.
     */
    public Forest(final Forest original) {
        this.forest = original.forest;
        this.forestSize = original.forestSize;
    }

    /** getPercentBurned()
     * Returns the percentage of this forest's
     * trees that have been burnt down.
     */
    public double getPercentBurned() {
        int size = this.forestSize;
        // -1 to exclude the tree that started the fire
        double total = size * size - 1;
        double sum = 0;

        // calculate pecrent burned
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (this.forest[i][j].getState() == BURNT) {
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
    public boolean forestIsBurning() {
        int size = this.forestSize;

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (this.forest[i][j].getState() == SMOLDERING || this.forest[i][j].getState() == BURNING) {
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
    public void print() {
        int size = this.forestSize;

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (this.forest[i][j].getState() == BURNT) {
                    System.out.println(".");
                } else {
                    System.out.println("X");
                }
            }
            System.out.println("\n");
        }
    }

    /** fire_spreads()
     * Based on a random number and the probability of fire spreading,
     * determines whether a fire should spread or not.
     */
    public boolean fireSpreads(double probSpread) {
        double random = Math.random();
        if (random < probSpread) {
            return true;
        } else {
            return false;
        }
    }

    /** treeBurns()
     * Burn for one iteration to see how this tree's state should change.
     */
    public void treeBurns(int row, int column, double probSpread) {
        if (this.forestSize == 0) {
            // handle error
            throw new IllegalArgumentException("Forest.treeBurns(): This forest has not been initialized");
        }

        if (this.forest[row][column].getState() == BURNING) {
            if (row != 0) 
            { // North
                if (fireSpreads(probSpread) && this.forest[row - 1][column].getState() == UNBURNT) 
                {
                    this.forest[row - 1][column].setState(SMOLDERING);
                }
            }
            if (row != this.forestSize - 1) 
            { // South
                if (fireSpreads(probSpread) && this.forest[row + 1][column].getState() == UNBURNT) 
                {
                    this.forest[row + 1][column].setState(SMOLDERING);
                }
            }   
            if (column != 0) 
            { // West
                if (fireSpreads(probSpread) && this.forest[row][column - 1].getState() == UNBURNT) 
                {
                    this.forest[row][column - 1].setState(SMOLDERING);
                }
            }
            if (column != this.forestSize - 1) 
            { // East
                if (fireSpreads(probSpread) && this.forest[row][column + 1].getState() == UNBURNT) 
                {
                    this.forest[row][column + 1].setState(SMOLDERING);
                }
            }   
        }
    }

    /** reset()
     * sets the state of every tree in
     * the forest to "unburnt".
     */
    public void reset() {
        int size = this.forestSize;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                this.forest[i][j].setState(UNBURNT);
            }
        }
    }

}