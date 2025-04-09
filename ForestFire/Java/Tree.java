/** Tree.java
 * Defines a class for individual "Tree" objects.
 * These Trees contian information about their individual state.
 * 
 * Tree methods add support for retrieving and modifying 
 * the "state" of the Tree.
 * 
 * Adapted for CSinParallel.org by Steven McKelvey, Calvin University, Summer 2024
 * Based on firestarter.c by David Joiner
 */

public class Tree {
    private static final int UNBURNT = 0;
    private static final int SMOLDERING = 1;
    private static final int BURNING = 2;
    private static final int BURNT = 3;

    private int state;

    /** default constructor
     * POSTCONDITION: state == UNBURNT
     */
    public Tree() {
        this.state = UNBURNT;
    }

    /** copy constructor
     * @param original: a Tree
     * 
     * POSTCONDITION: I am a distinct copy of original.
     */
    public Tree(final Tree original) {
        this.state = original.getState();
    }

    public int getState() {
        return this.state;
    }

    public void setState(int newState) {
        this.state = newState;
    }

    /** light()
     * Lights a tree on fire (sets it to smoldering)
     */
    public void light() {
        this.setState(SMOLDERING);
    }

    /** continueBurning()
     * used to determine whether a tree's state should be
     * changed to "BURNING" or "BURNT".
     */
    public void continueBurning() {
        // a burning tree burns down, a smoldering tree ignites
        if (this.getState() == BURNING) {
            this.setState(BURNT);
        } else if (this.getState() == SMOLDERING) {
            this.setState(BURNING);
        }
    }
}