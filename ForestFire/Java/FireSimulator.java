/** FireSimulator.java
 * Defines a class for running forest fire simulations
 * The simulator object contains a forest object and
 * the size of the forest the simulation is being run on.
 *
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
 * Based on firestarter.c by David Joiner
 */

public class FireSimulator {
    public int forestSize;
    public Forest forest;

    public FireSimulator(int forestSize) {
        this.forestSize = forestSize;
        this.forest = new Forest(forestSize);
    }

    /** run()
     * runs one trial of the fire simulation.
     * Returns the number of iterations the simulation ran for.
     */
    public int run(double probSpread, int startRow, int startColumn) {
        this.forest.reset();
        this.forest.forest[startRow][startColumn].light();

        int count = 0;
        int size = this.forestSize;
        while (this.forest.forestIsBurning()) {
            // run iteration
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    this.forest.forest[i][j].continueBurning();
                }
            }
            for (int i = 0; i < size; ++i) {
                for (int j = 0; j < size; ++j) {
                    this.forest.treeBurns(i, j, probSpread);
                }
            }
            ++count;
        }

        return count;
    }

    public double getPercentBurned() {
        return this.forest.getPercentBurned();
    }
}